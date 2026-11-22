#ifndef KTAILCTL_TAILDROP_SENDER_HPP
#define KTAILCTL_TAILDROP_SENDER_HPP

#include <QList>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>

class TaildropSendJob;

/**
 * Entry point for sending files via Taildrop.
 *
 * Creates and starts a TaildropSendJob per request, keeps track of the running jobs and notifies the user once a job finishes. The jobAdded()/jobFinished()
 * signals and jobs() are meant as the foundation for a future transfer overview in the UI.
 */
class TaildropSender : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Access the TaildropSender via Tailscale.taildropSender")

    Q_PROPERTY(int activeJobCount READ activeJobCount NOTIFY activeJobCountChanged)

public:
    explicit TaildropSender(QObject *parent = nullptr);
    /// Cancels all running transfers, so they do not keep the worker threads (and thus application shutdown) busy.
    ~TaildropSender() override;
    TaildropSender(const TaildropSender &) = delete;
    TaildropSender(TaildropSender &&) = delete;
    auto operator=(const TaildropSender &) -> TaildropSender & = delete;
    auto operator=(TaildropSender &&) -> TaildropSender & = delete;

    /// Send local files to the peer with the given stable node ID. peerName is only used for display purposes.
    auto sendFiles(const QString &peerId, const QString &peerName, const QStringList &files) -> TaildropSendJob *;

    /// QML-friendly overload of sendFiles() taking (local) file URLs as returned by a FileDialog.
    Q_INVOKABLE void sendFileUrls(const QString &peerId, const QString &peerName, const QList<QUrl> &urls);

    /// Currently running jobs, oldest first.
    [[nodiscard]] auto jobs() const -> QList<TaildropSendJob *>;
    [[nodiscard]] auto activeJobCount() const -> int;

Q_SIGNALS:
    void jobAdded(TaildropSendJob *job);
    void jobFinished(TaildropSendJob *job);
    void activeJobCountChanged();

private:
    void onJobResult(TaildropSendJob *job);

    QList<QPointer<TaildropSendJob>> mJobs;
};

#endif // KTAILCTL_TAILDROP_SENDER_HPP
