#include "pinger.hpp"

#include <QByteArray>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QPointer>
#include <QStringLiteral>
#include <QTimer>
#include <ThreadWeaver/ThreadWeaver>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <libktailctl_wrapper.h>
#include <memory>
#include <utility>

namespace
{
// Runs on a ThreadWeaver worker thread: performs exactly one blocking ping via
// the Go wrapper and parses its JSON result into a PingSample.
auto performPing(const QString &address, double timeoutSeconds) -> PingSample
{
    PingSample sample;
    sample.time = QDateTime::currentDateTimeUtc();

    const QByteArray addrUtf8 = address.toUtf8();
    GoString goAddr;
    goAddr.p = addrUtf8.constData();
    goAddr.n = static_cast<GoInt>(addrUtf8.size());

    const std::unique_ptr<char, decltype(&free)> json_str(tailscale_ping(&goAddr, timeoutSeconds), &free);
    if (!json_str) {
        sample.error = QStringLiteral("ping failed");
        return sample;
    }

    const QByteArray buffer(json_str.get(), static_cast<qsizetype>(strlen(json_str.get())));
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(buffer, &error);
    if (error.error != QJsonParseError::NoError) {
        sample.error = error.errorString();
        return sample;
    }

    const QJsonObject obj = doc.object();
    sample.success = obj.value(QStringLiteral("success")).toBool();
    sample.latencyMs = obj.value(QStringLiteral("latencyMs")).toDouble();
    sample.via = obj.value(QStringLiteral("via")).toString();
    sample.nodeName = obj.value(QStringLiteral("nodeName")).toString();
    sample.error = obj.value(QStringLiteral("error")).toString();
    return sample;
}
}

Pinger::Pinger(QString address, QObject *parent)
    : QObject(parent)
    , mAddress(std::move(address))
    , mTimer(new QTimer(this))
{
    mTimer->setSingleShot(true);
    connect(mTimer, &QTimer::timeout, this, &Pinger::pingNow);
}

void Pinger::start()
{
    if (mRunning) {
        return;
    }
    mRunning = true;
    Q_EMIT runningChanged();
    pingNow(); // ping immediately, then keep going on the timer
}

void Pinger::stop()
{
    if (!mRunning) {
        return;
    }
    mRunning = false;
    mTimer->stop();
    Q_EMIT runningChanged();
    // An already in-flight ping may still deliver one final sample; that is
    // harmless because onSampleReady() only reschedules while mRunning is true.
}

void Pinger::toggle()
{
    if (mRunning) {
        stop();
    } else {
        start();
    }
}

void Pinger::clearHistory()
{
    mHistory.clear();
    mMaxLatencyMs = 0.0;
    mHasResult = false;
    Q_EMIT historyChanged();
    Q_EMIT lastResultChanged();
}

void Pinger::pingNow()
{
    if (mInFlight) {
        return;
    }
    mInFlight = true;

    const QString address = mAddress;
    const double timeout = mTimeoutSeconds;
    const QPointer<Pinger> self(this);

    // Run the blocking ping on a ThreadWeaver worker, then hop back to the main
    // thread (guarded by a QPointer) to deliver the result safely.
    ThreadWeaver::Queue::instance()->enqueue(ThreadWeaver::make_job([address, timeout, self]() {
        const PingSample sample = performPing(address, timeout);
        QMetaObject::invokeMethod(qApp, [self, sample]() {
            if (self) {
                self->onSampleReady(sample);
            }
        });
    }));
}

void Pinger::onSampleReady(const PingSample &sample)
{
    mInFlight = false;
    mLast = sample;
    mHasResult = true;
    Q_EMIT lastResultChanged();

    mHistory.append(sample.success ? sample.latencyMs : -1.0);
    while (mHistory.size() > MaxHistory) {
        mHistory.removeFirst();
    }

    double maxLatency = 0.0;
    for (const QVariant &value : std::as_const(mHistory)) {
        maxLatency = std::max(maxLatency, value.toDouble());
    }
    mMaxLatencyMs = maxLatency;
    Q_EMIT historyChanged();

    if (mRunning) {
        mTimer->start(mIntervalMs);
    }
}
