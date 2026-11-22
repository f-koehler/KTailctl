#include "taildrop_send_job.hpp"

#include <KJob>
#include <QByteArray>
#include <QCoreApplication>
#include <QFileInfo>
#include <QLoggingCategory>
#include <QMessageLogger>
#include <QMetaObject>
#include <QPair>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QStringLiteral>
#include <QTimer>
#include <ThreadWeaver/Queue>
#include <ThreadWeaver/ThreadWeaver>
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <libktailctl_wrapper.h>
#include <memory>
#include <mutex>
#include <numeric>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <utility>

#include "logging_taildrop.hpp"

namespace Taildrop
{
/**
 * State shared between a TaildropSendJob (main thread) and its worker (ThreadWeaver thread) and, through the progress callback, the Go wrapper.
 *
 * It is reference counted so that it outlives the KJob if the job is killed and deleted while a transfer is still winding down.
 */
// NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
struct SendJobState {
    explicit SendJobState(quint64 jobId)
        : id(jobId)
    {
    }

    /// Job ID used to address this transfer in the Go wrapper.
    const quint64 id;
    /// Index of the file currently being transferred (== number of files completed).
    std::atomic<qsizetype> currentFile{0};
    /// Bytes of the current file sent so far, updated from a Go thread.
    std::atomic<qint64> currentFileBytes{0};

    /// Cancel the running and all following transfers.
    void cancel()
    {
        const std::scoped_lock lock(mutex);
        if (!released) {
            tailscale_taildrop_cancel(id);
        }
    }

    /// Release the job in the Go wrapper. After this, cancel() is a no-op.
    void release()
    {
        const std::scoped_lock lock(mutex);
        if (!released) {
            released = true;
            tailscale_taildrop_release(id);
        }
    }

private:
    std::mutex mutex;
    bool released = false;
};
// NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
} // namespace Taildrop

namespace
{
auto nextJobId() -> quint64
{
    static std::atomic<quint64> counter{0};
    return ++counter;
}

// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
struct GoStringHolder {
    explicit GoStringHolder(const QString &string)
        : utf8(string.toUtf8())
        , go{.p = utf8.constData(), .n = static_cast<GoInt>(utf8.size())}
    {
    }
    QByteArray utf8;
    GoString go;
};
// NOLINTEND(misc-non-private-member-variables-in-classes)

// Invoked by the Go wrapper on one of its threads whenever more of the current file has been sent.
extern "C" void taildropProgressCallback(void *userdata, int64_t bytesSent)
{
    static_cast<Taildrop::SendJobState *>(userdata)->currentFileBytes.store(bytesSent, std::memory_order_relaxed);
}

struct WorkerResult {
    qsizetype filesSent = 0;
    QString errorMessage;
};

// Runs on a ThreadWeaver worker thread: sends all files one after another, stopping at the first failure.
auto sendFiles(Taildrop::SendJobState &state, const QString &peerId, const QStringList &files) -> WorkerResult
{
    WorkerResult result;
    GoStringHolder goPeerId(peerId);
    for (const QString &file : files) {
        state.currentFileBytes.store(0, std::memory_order_relaxed);
        GoStringHolder goPath(file);
        const std::unique_ptr<char, decltype(&free)> error(tailscale_taildrop_send_file(state.id, &goPeerId.go, &goPath.go, &taildropProgressCallback, &state),
                                                           &free);
        if (error) {
            result.errorMessage = QString::fromUtf8(error.get());
            return result;
        }
        ++result.filesSent;
        state.currentFile.store(result.filesSent, std::memory_order_relaxed);
    }
    return result;
}
} // namespace

TaildropSendJob::TaildropSendJob(QString peerId, QString peerName, QStringList files, QObject *parent)
    : KJob(parent)
    , mPeerId(std::move(peerId))
    , mPeerName(std::move(peerName))
    , mFiles(std::move(files))
    , mProgressTimer(new QTimer(this))
    , mState(std::make_shared<Taildrop::SendJobState>(nextJobId()))
{
    setCapabilities(KJob::Killable);
    mProgressTimer->setInterval(ProgressIntervalMs);
    connect(mProgressTimer, &QTimer::timeout, this, &TaildropSendJob::updateProgress);
}

TaildropSendJob::~TaildropSendJob()
{
    if (!mWorkerStarted) {
        // The worker was never started, so nobody else will release the job in the Go wrapper.
        mState->release();
    }
}

void TaildropSendJob::start()
{
    if (mFiles.isEmpty()) {
        emitResult();
        return;
    }

    mFileSizes.reserve(mFiles.size());
    for (const QString &file : std::as_const(mFiles)) {
        mFileSizes.append(QFileInfo(file).size());
    }
    setTotalAmount(KJob::Files, static_cast<qulonglong>(mFiles.size()));
    setTotalAmount(KJob::Bytes, static_cast<qulonglong>(std::accumulate(mFileSizes.cbegin(), mFileSizes.cend(), qint64{0})));
    Q_EMIT description(this, tr("Sending files via Taildrop"), qMakePair(tr("To"), mPeerName));
    updateProgress();
    mProgressTimer->start();

    qCInfo(Logging::Taildrop) << "Sending" << mFiles.size() << "file(s) to" << mPeerName << mPeerId;

    const std::shared_ptr<Taildrop::SendJobState> state = mState;
    const QString peerId = mPeerId;
    const QStringList files = mFiles;
    const QPointer<TaildropSendJob> self(this);
    mWorkerStarted = true;
    ThreadWeaver::Queue::instance()->enqueue(ThreadWeaver::make_job([state, peerId, files, self]() -> void {
        const WorkerResult result = sendFiles(*state, peerId, files);
        state->release();
        // Hop back to the main thread; the job may have been killed and deleted in the meantime.
        QMetaObject::invokeMethod(qApp, [self, result]() -> void {
            if (self) {
                self->onWorkerFinished(result.filesSent, result.errorMessage);
            }
        });
    }));
}

auto TaildropSendJob::doKill() -> bool
{
    qCInfo(Logging::Taildrop) << "Cancelling transfer to" << mPeerName;
    mProgressTimer->stop();
    mState->cancel();
    return true;
}

void TaildropSendJob::updateProgress()
{
    const qsizetype currentFile = mState->currentFile.load(std::memory_order_relaxed);
    const qint64 currentFileBytes = mState->currentFileBytes.load(std::memory_order_relaxed);

    qint64 bytes = std::accumulate(mFileSizes.cbegin(), mFileSizes.cbegin() + std::min(currentFile, mFileSizes.size()), qint64{0});
    if (currentFile < mFileSizes.size()) {
        bytes += std::min(currentFileBytes, mFileSizes.at(currentFile));
        if (currentFile != mLastReportedFile) {
            mLastReportedFile = currentFile;
            Q_EMIT infoMessage(this, tr("Sending %1").arg(QFileInfo(mFiles.at(currentFile)).fileName()));
        }
    }

    setProcessedAmount(KJob::Files, static_cast<qulonglong>(currentFile));
    setProcessedAmount(KJob::Bytes, static_cast<qulonglong>(bytes));
}

void TaildropSendJob::onWorkerFinished(qsizetype filesSent, const QString &errorMessage)
{
    mProgressTimer->stop();
    mFilesSent = filesSent;
    updateProgress();

    if (!errorMessage.isEmpty()) {
        const QString file = filesSent < mFiles.size() ? QFileInfo(mFiles.at(filesSent)).fileName() : QString();
        qCWarning(Logging::Taildrop) << "Failed to send" << file << "to" << mPeerName << ":" << errorMessage;
        setError(KJob::UserDefinedError);
        setErrorText(tr("Failed to send %1 to %2: %3").arg(file, mPeerName, errorMessage));
    } else {
        qCInfo(Logging::Taildrop) << "Sent" << filesSent << "file(s) to" << mPeerName;
    }
    emitResult();
}
