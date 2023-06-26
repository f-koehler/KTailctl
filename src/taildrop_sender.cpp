#include "taildrop_sender.h"
#include <QDir>
#include <QFileDialog>
#include <libtailctlpp.h>

TaildropSendThread::TaildropSendThread(const QString &target, const QStringList &files, QObject *parent)
    : QThread(parent)
    , mTarget(target)
    , mFiles(files)
{
}

void TaildropSendThread::run()
{
    QByteArray targetBytes = mTarget.toUtf8();
    GoString target{targetBytes.constData(), targetBytes.length()};

    QByteArray fileBytes;
    GoString file;
    for (const auto &f : mFiles) {
        QByteArray fileBytes = f.toUtf8();
        file.p = fileBytes.constData();
        file.n = fileBytes.length();
        tailscale_send_file(target, file);
    }
}

TaildropSendJob::TaildropSendJob(const QString &target, const QStringList &files, QObject *parent)
    : KJob(parent)
{
    mThread = new TaildropSendThread(target, files, this);
    connect(mThread, &TaildropSendThread::finished, this, &TaildropSendJob::emitResult);
}

TaildropSendJob *TaildropSendJob::selectAndSendFiles(const QString &target)
{
    auto job = new TaildropSendJob(target, QFileDialog::getOpenFileNames(nullptr, "Select files to send", QDir::homePath()));
    job->start();
    return job;
}

void TaildropSendJob::start()
{
    mThread->start();
}

QmlTaildropSender::QmlTaildropSender(QObject *parent)
    : QObject(parent)
{
}

void QmlTaildropSender::selectAndSendFiles(const QString &target)
{
    TaildropSendJob::selectAndSendFiles(target);
}
