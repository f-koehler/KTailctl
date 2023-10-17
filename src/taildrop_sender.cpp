#include "taildrop_sender.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <libtailwrap.h>
#include <utility>

TaildropSendThread::TaildropSendThread(QString target, const QStringList &files, QObject *parent)
    : QThread(parent)
    , mTarget(std::move(target))
    , mFiles(files)
    , mBytesSent(0UL)
    , mBytesTotal(0UL)
    , mCurrentFileBytesSent(0UL)
{
    for (const auto &file : files) {
        mBytesTotal += static_cast<quint64>(QFileInfo(file).size());
    }
}

void TaildropSendThread::run()
{
    QByteArray const targetBytes = mTarget.toUtf8();
    GoString const target{targetBytes.constData(), targetBytes.length()};

    QByteArray const fileBytes;
    GoString file;
    for (const auto &filename : mFiles) {
        mCurrentFileBytesSent = 0UL;
        QByteArray const fileBytes = filename.toUtf8();
        file.p = fileBytes.constData();
        file.n = fileBytes.length();
        tailscale_send_file(target, file, [](unsigned long n) {
            qDebug() << "Bytes sent" << n;
        });
        mBytesSent += static_cast<quint64>(QFileInfo(filename).size());
    }
    mBytesSent = mBytesTotal;
}

TaildropSendJob::TaildropSendJob(const QString &target, const QStringList &files, QObject *parent)
    : KJob(parent)
    , mThread(new TaildropSendThread(target, files, this))
{
    connect(mThread, &TaildropSendThread::finished, this, &TaildropSendJob::emitResult);
}

TaildropSendJob *TaildropSendJob::selectAndSendFiles(const QString &target)
{
    auto *job = new TaildropSendJob(target, QFileDialog::getOpenFileNames(nullptr, "Select files to send", QDir::homePath()));
    job->start();
    return job;
}
TaildropSendJob *TaildropSendJob::sendFiles(const QString &target, const QStringList &files)
{
    auto *job = new TaildropSendJob(target, files);
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
void QmlTaildropSender::sendFiles(const QString &target, const QStringList &files)
{
    TaildropSendJob::sendFiles(target, files);
}
