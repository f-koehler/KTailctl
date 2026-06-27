#ifndef KTAILCTL_PINGER_HPP
#define KTAILCTL_PINGER_HPP

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>

class QTimer;

// A single Tailscale-layer ping outcome. Produced on a worker thread by
// performPing() and delivered back to the owning Pinger on the main thread.
struct PingSample {
    bool success = false;
    double latencyMs = 0.0;
    QString via;
    QString nodeName;
    QString error;
    QDateTime time;
};

// Drives a continuous Tailscale ping against a single address. The individual
// pings run on a background thread (via KF6::ThreadWeaver) so the UI never
// blocks while waiting for a (possibly timing out) reply. Results are folded
// into a bounded latency history suitable for a live graph in QML, and the
// most recent sample is exposed through dedicated properties.
//
// Instances are not creatable from QML; obtain one via Tailscale.pinger(address).
class Pinger : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Obtain a Pinger via Tailscale.pinger(address)")

    Q_PROPERTY(QString address READ address CONSTANT)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(bool hasResult READ hasResult NOTIFY lastResultChanged)
    Q_PROPERTY(bool lastSuccess READ lastSuccess NOTIFY lastResultChanged)
    Q_PROPERTY(double lastLatencyMs READ lastLatencyMs NOTIFY lastResultChanged)
    Q_PROPERTY(QString lastVia READ lastVia NOTIFY lastResultChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastResultChanged)
    Q_PROPERTY(QDateTime lastTime READ lastTime NOTIFY lastResultChanged)
    // Latency samples in milliseconds, oldest first. A value of -1 marks a
    // failed/timed-out sample so the graph can render a gap.
    Q_PROPERTY(QVariantList history READ history NOTIFY historyChanged)
    // Epoch-millisecond timestamps parallel to history, so the graph can plot
    // samples against wall-clock time and show gaps for paused periods.
    Q_PROPERTY(QVariantList historyTimes READ historyTimes NOTIFY historyChanged)
    Q_PROPERTY(double maxLatencyMs READ maxLatencyMs NOTIFY historyChanged)
    // Milliseconds between consecutive pings while running.
    Q_PROPERTY(int intervalMs READ intervalMs CONSTANT)

public:
    // Maximum number of samples retained for the live graph.
    static constexpr int MaxHistory = 60;
    // Per-ping timeout and the pause between consecutive pings while running.
    static constexpr double DefaultTimeoutSeconds = 5.0;
    static constexpr int DefaultIntervalMs = 1000;

    explicit Pinger(QString address, QObject *parent = nullptr);

    [[nodiscard]] auto address() const noexcept -> const QString &
    {
        return mAddress;
    }
    [[nodiscard]] auto running() const noexcept -> bool
    {
        return mRunning;
    }
    [[nodiscard]] auto hasResult() const noexcept -> bool
    {
        return mHasResult;
    }
    [[nodiscard]] auto lastSuccess() const noexcept -> bool
    {
        return mLast.success;
    }
    [[nodiscard]] auto lastLatencyMs() const noexcept -> double
    {
        return mLast.latencyMs;
    }
    [[nodiscard]] auto lastVia() const noexcept -> const QString &
    {
        return mLast.via;
    }
    [[nodiscard]] auto lastError() const noexcept -> const QString &
    {
        return mLast.error;
    }
    [[nodiscard]] auto lastTime() const noexcept -> const QDateTime &
    {
        return mLast.time;
    }
    [[nodiscard]] auto history() const noexcept -> const QVariantList &
    {
        return mHistory;
    }
    [[nodiscard]] auto historyTimes() const noexcept -> const QVariantList &
    {
        return mHistoryTimes;
    }
    [[nodiscard]] auto maxLatencyMs() const noexcept -> double
    {
        return mMaxLatencyMs;
    }
    [[nodiscard]] auto intervalMs() const noexcept -> int
    {
        return mIntervalMs;
    }

    Q_SLOT Q_INVOKABLE void start();
    Q_SLOT Q_INVOKABLE void stop();
    Q_SLOT Q_INVOKABLE void toggle();
    Q_SLOT Q_INVOKABLE void clearHistory();

    // Invoked on the main thread once a worker ping finishes.
    void onSampleReady(const PingSample &sample);

Q_SIGNALS:
    void runningChanged();
    void lastResultChanged();
    void historyChanged();

private:
    void pingNow();

    QString mAddress;
    bool mRunning = false;
    bool mHasResult = false;
    bool mInFlight = false;
    double mTimeoutSeconds = DefaultTimeoutSeconds;
    int mIntervalMs = DefaultIntervalMs;
    PingSample mLast;
    QVariantList mHistory;
    QVariantList mHistoryTimes;
    double mMaxLatencyMs = 0.0;
    QTimer *mTimer;
};

#endif // KTAILCTL_PINGER_HPP
