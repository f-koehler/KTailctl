#include "taildrop_sender.h"
#include <QDir>
#include <QFileDialog>
#include <libtailctlpp.h>

TaildropSender::TaildropSender(QObject *parent)
    : QObject(parent)
{
}
TaildropSender::TaildropSender(const QString &target, QObject *parent)
    : QObject(parent)
    , mTarget(target)
{
}
int TaildropSender::workerCount()
{
    return mWorkers.count();
}

void TaildropSender::selectAndSendFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(nullptr, "Select files to send", QDir::homePath());
    if (files.isEmpty()) {
        return;
    }
    sendFiles(files);
}

void TaildropSender::pruneWorkers()
{
    QMutableListIterator<TaildropSendWorker *> it(mWorkers);
    bool countChanged = false;
    while (it.hasNext()) {
        if (it.next()->isFinished()) {
            it.remove();
            countChanged = true;
        }
    }
    if (countChanged) {
        emit workerCountChanged(mWorkers.count());
    }
}

void TaildropSender::sendFiles(const QStringList &files)
{
    auto *worker = new TaildropSendWorker(mTarget, files, this);
    QObject::connect(worker, &TaildropSendWorker::finished, this, &TaildropSender::pruneWorkers);
    mWorkers.append(worker);
    emit workerCountChanged(mWorkers.count());
}

TaildropSendWorker::TaildropSendWorker(const QString &target, const QStringList &files, QObject *parent)
    : QThread(parent)
    , mTarget(target)
    , mFiles(files)
{
    start();
}

void TaildropSendWorker::run()
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
