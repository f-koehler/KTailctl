#include "taildrop_send_job.h"
#include "libktailctl_wrapper.h"

#include <QLoggingCategory>
#include <QMap>
#include <QMutexLocker>
#include <QTimer>

Q_LOGGING_CATEGORY(logcat_taildrop_send_job, "org.fkoehler.KTailctl.TaildropSendJob")

TaildropSendJob::TaildropSendJob(const QString &target, QObject *parent)
    : KJob(parent)
    , mTarget(target.toUtf8())
{
    setProgressUnit(Files);
    setTotalAmount(Files, 0);
    setProcessedAmount(Files, 0);
}

TaildropSendJob *TaildropSendJob::addFiles(const QList<QUrl> &files)
{
    QMutexLocker locker(&mMutex);
    for (const QUrl &file : files) {
        mFiles.enqueue(file);
    }
    setTotalAmount(Files, totalAmount(Files) + files.size());
    return this;
}

void TaildropSendJob::start()
{
    QTimer::singleShot(0, this, &TaildropSendJob::sendFiles);
}

bool TaildropSendJob::isRunning() const
{
    return mRunning;
}

void TaildropSendJob::sendFiles()
{
    GoString targetGo = {mTarget.constData(), mTarget.size()};
    QByteArray file;
    GoString fileGo;
    mRunning = true;
    emit isRunningChanged(true);

    while (true) {
        mMutex.lock();
        if (mFiles.isEmpty()) {
            mMutex.unlock();
            emitResult();
            mRunning = false;
            emit isRunningChanged(false);
            return;
        }
        file = mFiles.dequeue().path().toUtf8();
        fileGo = {file.constData(), file.size()};
        mMutex.unlock();

        qCInfo(logcat_taildrop_send_job) << "Sending file" << fileGo.p << "to" << targetGo.p;

        mRunning = true;
        tailscale_send_file(targetGo, fileGo, [](unsigned long n) {
            qDebug() << "Bytes sent" << n;
        });
        mRunning = false;

        setProcessedAmount(Files, processedAmount(Files) + 1);
    }
}

void TaildropSendJob::clear()
{
    QTimer::singleShot(10000, [this]() {
        QMutexLocker locker(&mMutex);
        if (!mFiles.isEmpty()) {
            return;
        }
        mFiles.clear();
        this->setTotalAmount(Files, 0);
        this->setProcessedAmount(Files, 0);
    });
}

TaildropSendJob *TaildropSendJob::getJob(const QString &target)
{
    static QMutex mutex;
    static QMap<QString, TaildropSendJob *> jobs;

    QMutexLocker locker(&mutex);

    const auto pos = jobs.find(target);
    if (pos != jobs.end()) {
        return *pos;
    }

    TaildropSendJob *job = new TaildropSendJob(target);
    jobs.insert(target, job);
    return job;
}