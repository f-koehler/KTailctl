#ifndef KTAILCTL_TAILDROP_SEND_JOB_HPP
#define KTAILCTL_TAILDROP_SEND_JOB_HPP

#include <KJob>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <memory>
#include <qtmetamacros.h>
#include <qtypes.h>

class QTimer;

namespace Taildrop
{
struct SendJobState;
}

/**
 * Sends one or more local files to a single peer via Taildrop.
 *
 * The files are transferred one after another on a ThreadWeaver worker thread, since each transfer is a blocking call into the Go wrapper. Progress is
 * reported through the regular KJob API: the total/processed amounts in KJob::Bytes and KJob::Files, percent() (based on bytes), description() and
 * infoMessage(). This makes it straightforward to register the job with a KJobTrackerInterface (e.g. KUiServerV2JobTracker for Plasma's progress
 * notifications) or to show it in our own UI later on.
 *
 * Killing the job cancels the running transfer. Like every KJob, it deletes itself after emitting result() unless setAutoDelete(false) was called.
 */
class TaildropSendJob : public KJob
{
    Q_OBJECT

public:
    /// Interval in which progress is pulled from the worker thread and published via the KJob API.
    static constexpr int ProgressIntervalMs = 250;

    TaildropSendJob(QString peerId, QString peerName, QStringList files, QObject *parent = nullptr);
    ~TaildropSendJob() override;
    TaildropSendJob(const TaildropSendJob &) = delete;
    TaildropSendJob(TaildropSendJob &&) = delete;
    auto operator=(const TaildropSendJob &) -> TaildropSendJob & = delete;
    auto operator=(TaildropSendJob &&) -> TaildropSendJob & = delete;

    void start() override;

    [[nodiscard]] auto peerId() const noexcept -> const QString &
    {
        return mPeerId;
    }
    [[nodiscard]] auto peerName() const noexcept -> const QString &
    {
        return mPeerName;
    }
    [[nodiscard]] auto files() const noexcept -> const QStringList &
    {
        return mFiles;
    }
    /// Number of files that were sent successfully so far.
    [[nodiscard]] auto filesSent() const noexcept -> qsizetype
    {
        return mFilesSent;
    }

protected:
    auto doKill() -> bool override;

private:
    void updateProgress();
    void onWorkerFinished(qsizetype filesSent, const QString &errorMessage);

    QString mPeerId;
    QString mPeerName;
    QStringList mFiles;
    QList<qint64> mFileSizes;
    qsizetype mFilesSent = 0;
    qsizetype mLastReportedFile = -1;
    bool mWorkerStarted = false;
    QTimer *mProgressTimer;
    std::shared_ptr<Taildrop::SendJobState> mState;
};

#endif // KTAILCTL_TAILDROP_SEND_JOB_HPP
