#include "taildrop_sender.hpp"

#include <KJob>
#include <KNotification>
#include <QFileInfo>
#include <QList>
#include <QLoggingCategory>
#include <QMessageLogger>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringLiteral>
#include <QUrl>
#include <qtmetamacros.h>

#include "logging_taildrop.hpp"
#include "taildrop_send_job.hpp"

TaildropSender::TaildropSender(QObject *parent)
    : QObject(parent)
{
}

TaildropSender::~TaildropSender()
{
    for (TaildropSendJob *job : jobs()) {
        job->kill(KJob::Quietly);
    }
}

auto TaildropSender::sendFiles(const QString &peerId, const QString &peerName, const QStringList &files) -> TaildropSendJob *
{
    if (files.isEmpty()) {
        return nullptr;
    }

    auto *job = new TaildropSendJob(peerId, peerName, files, this);
    connect(job, &KJob::result, this, [this, job]() -> void {
        onJobResult(job);
    });
    mJobs.append(job);
    Q_EMIT jobAdded(job);
    Q_EMIT activeJobCountChanged();
    job->start();
    return job;
}

void TaildropSender::sendFileUrls(const QString &peerId, const QString &peerName, const QList<QUrl> &urls)
{
    QStringList files;
    files.reserve(urls.size());
    for (const QUrl &url : urls) {
        if (!url.isLocalFile()) {
            qCWarning(Logging::Taildrop) << "Ignoring non-local file" << url;
            continue;
        }
        files.append(url.toLocalFile());
    }
    sendFiles(peerId, peerName, files);
}

auto TaildropSender::jobs() const -> QList<TaildropSendJob *>
{
    QList<TaildropSendJob *> result;
    result.reserve(mJobs.size());
    for (const auto &job : mJobs) {
        if (job) {
            result.append(job);
        }
    }
    return result;
}

auto TaildropSender::activeJobCount() const -> int
{
    return static_cast<int>(jobs().size());
}

void TaildropSender::onJobResult(TaildropSendJob *job)
{
    mJobs.removeAll(job);
    Q_EMIT jobFinished(job);
    Q_EMIT activeJobCountChanged();

    if (job->error() == KJob::KilledJobError) {
        return;
    }
    if (job->error() != KJob::NoError) {
        KNotification::event(KNotification::Error, tr("Taildrop failed"), job->errorText(), QStringLiteral("document-send"));
        return;
    }

    const QString text = job->files().size() == 1 ? tr("Sent %1 to %2").arg(QFileInfo(job->files().constFirst()).fileName(), job->peerName())
                                                  : tr("Sent %1 files to %2").arg(job->files().size()).arg(job->peerName());
    KNotification::event(KNotification::Notification, tr("Taildrop"), text, QStringLiteral("document-send"));
}
