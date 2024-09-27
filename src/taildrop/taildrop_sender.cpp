#include "taildrop_sender.hpp"
#include "libktailctl_wrapper.h"
#include <QFileDialog>
#include <QLoggingCategory>
#include <QTimer>

Q_LOGGING_CATEGORY(logcat_taildrop_send_job, "org.fkoehler.KTailctl.TaildropSendJob")

TaildropSendWorker::TaildropSendWorker(const QString &target, const QList<QUrl> &files, QObject *parent)
    : QThread(parent)
    , mTarget(target)
    , mFiles(files)
{
}

void TaildropSendWorker::run()
{
    const QByteArray targetBytes = mTarget.toUtf8();
    const GoString targetGo{targetBytes.constData(), targetBytes.size()};

    qsizetype fileCounter = 0;
    for (const QUrl &file : std::as_const(mFiles)) {
        const QByteArray fileBytes = file.path().toUtf8();
        const GoString fileGo{fileBytes.constData(), fileBytes.size()};
        qCInfo(logcat_taildrop_send_job) << "Sending file:" << fileGo.p << "to target:" << mTarget;

        tailscale_send_file(targetGo, fileGo, [](unsigned long n) {
            qDebug() << "Bytes sent:" << n;
        });
        qCInfo(logcat_taildrop_send_job) << "Completed sending file:" << fileGo.p << "to target:" << mTarget;

        ++fileCounter;
        emit filesSent(fileCounter);
    }
    emit sendingFinished();
}

TaildropSendJob::TaildropSendJob(const QString &target, const QList<QUrl> &files, QObject *parent)
    : KJob(parent)
    , mWorker(new TaildropSendWorker(target, files, this))
{
    setProgressUnit(KJob::Files);
    setTotalAmount(KJob::Files, files.size());
    setProcessedAmount(KJob::Files, 0);

    connect(mWorker, &TaildropSendWorker::sendingFinished, this, &TaildropSendJob::emitResult);
    connect(mWorker, &TaildropSendWorker::filesSent, this, [this](qsizetype filesSent) {
        setProcessedAmount(KJob::Files, filesSent);
    });
}

void TaildropSendJob::start()
{
    QTimer::singleShot(0, [this]() {
        mWorker->start();
    });
}

TaildropSendJobFactory *TaildropSendJobFactory::instance()
{
    static TaildropSendJobFactory instance;
    return &instance;
}

TaildropSendJob *TaildropSendJobFactory::sendFiles(const QString &target, const QList<QUrl> &files)
{
    auto *job = new TaildropSendJob(target, files, instance());
    job->start();
    return job;
}

TaildropSendJob *TaildropSendJobFactory::selectAndSendFiles(const QString &target)
{
    return sendFiles(target, QFileDialog::getOpenFileUrls(nullptr, "Select file(s) to send", QDir::homePath(), "All files (*)"));
}
