#include "taildrop_sender.hpp"
#include "libktailctl_wrapper.h"
#include <QFileDialog>
#include <QLoggingCategory>
#include <QTimer>

Q_LOGGING_CATEGORY(logcat_taildrop_send_job, "org.fkoehler.KTailctl.TaildropSendJob")

TaildropSendJob::TaildropSendJob(const QString &target, const QList<QUrl> &files, QObject *parent)
    : KJob(parent)
    , mTarget(target)
    , mFiles(files)
{
}

void TaildropSendJob::sendFiles()
{
    setProgressUnit(Files);
    setTotalAmount(Files, mFiles.size());
    setProcessedAmount(Files, 0);

    const QByteArray targetBytes = mTarget.toUtf8();
    const GoString targetGo{targetBytes.constData(), targetBytes.size()};
    qulonglong counter = 0;

    for (const QUrl &file : std::as_const(mFiles)) {
        const QByteArray fileBytes = file.path().toUtf8();
        const GoString fileGo{fileBytes.constData(), fileBytes.size()};
        qCInfo(logcat_taildrop_send_job) << "Sending file:" << fileGo.p << "to target:" << mTarget;

        tailscale_send_file(targetGo, fileGo, [](unsigned long n) {
            qDebug() << "Bytes sent:" << n;
        });
        ++counter;
        setTotalAmount(Files, counter);
        qCInfo(logcat_taildrop_send_job) << "Completed sending file:" << fileGo.p << "to target:" << mTarget;
    }
    emitResult();
}

void TaildropSendJob::start()
{
    QTimer::singleShot(0, this, &TaildropSendJob::sendFiles);
}

TaildropSender::TaildropSender(QObject *parent)
    : QObject(parent)
    , mMutex()
{
}

void TaildropSender::sendFiles(const QString &target, const QList<QUrl> &files)
{
    const QMutexLocker locker(&mMutex);
    auto *job = new TaildropSendJob(target, files, this);
    if (!mPeerJobs.contains(target)) {
        mPeerJobs.insert(target, QList<TaildropSendJob *>())->push_back(job);
    }
    job->start();
    QObject::connect(job, &KJob::result, this, &TaildropSender::cleanupJobs);
}

void TaildropSender::selectAndSendFiles(const QString &target)
{
    sendFiles(target, QFileDialog::getOpenFileUrls(nullptr, "Select file(s) to send", QDir::homePath(), "All files (*)"));
}

TaildropSender *TaildropSender::instance()
{
    static TaildropSender instance;
    return &instance;
}

void TaildropSender::cleanupJobs()
{
    const QMutexLocker locker(&mMutex);
    for (auto &jobs : mPeerJobs) {
        jobs.removeIf([](TaildropSendJob *job) {
            if (job->isFinished()) {
                job->deleteLater();
                return true;
            }
            return false;
        });
    }
}
