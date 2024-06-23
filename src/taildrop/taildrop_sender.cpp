#include "taildrop_sender.h"
#include "libktailctl_wrapper.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <utility>

Q_LOGGING_CATEGORY(logcat_taildrop_sender, "org.fkoehler.KTailctl.TaildropSender")

TaildropSendThread::TaildropSendThread(const QString &target, const QStringList &files, QObject *parent)
    : QThread(parent)
    , mTarget(std::move(target))
    , mFiles(files)
    , mBytesSent(0UL)
    , mBytesTotal(0UL)
    , mCurrentFileBytesSent(0UL)
{
    mBytesTotal = std::accumulate(files.begin(), files.end(), mBytesTotal, [](quint64 acc, const QString &file) {
        return acc + static_cast<quint64>(QFileInfo(file).size());
    });
}

void TaildropSendThread::run()
{
    QByteArray const targetBytes = mTarget.toUtf8();
    GoString const target{targetBytes.constData(), targetBytes.length()};

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

TaildropSender::TaildropSender(QObject *parent)
    : QObject(parent)
{
}

void TaildropSender::selectAndSendFiles(const QString &target)
{
    TaildropSendJob::selectAndSendFiles(target);
}
void TaildropSender::sendFiles(const QString &target, const QStringList &files)
{
    TaildropSendJob::sendFiles(target, files);
}
