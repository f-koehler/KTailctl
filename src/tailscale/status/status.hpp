#ifndef KTAILCTL_STATUS_HPP
#define KTAILCTL_STATUS_HPP

#include "client_version.hpp"
#include "exit_node_status.hpp"
#include "mullvad_exit_node_model.hpp"
#include "peer_status.hpp"
#include "property_list_model.hpp"
#include "self_hosted_exit_node_model.hpp"
#include "tailnet_status.hpp"
#include "user_profile.hpp"
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

using PeerModelBase = PropertyListModel<PeerStatus, PropertyListModelOwnership::External>;

class PeerModel : public PeerModelBase
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using PeerModelBase::PeerModelBase;
};

// https://pkg.go.dev/tailscale.com/ipn/ipnstate#Status
class Status : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    using PeerModel = ::PeerModel;

    enum class BackendState : uint8_t {
        NoState,
        NeedsLogin,
        NeedsMachineAuth,
        Stopped,
        Starting,
        Running
    };

    Q_ENUM(BackendState)

    Q_PROPERTY(QString version READ version BINDABLE bindableVersion NOTIFY versionChanged)
    Q_PROPERTY(bool isTun READ isTun BINDABLE bindableIsTun NOTIFY isTunChanged)
    Q_PROPERTY(BackendState backendState READ backendState BINDABLE bindableBackendState NOTIFY backendStateChanged)
    Q_PROPERTY(bool haveNodeKey READ haveNodeKey BINDABLE bindableHaveNodeKey NOTIFY haveNodeKeyChanged)
    Q_PROPERTY(QString authUrl READ authUrl BINDABLE bindableAuthUrl NOTIFY authUrlChanged)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps BINDABLE bindableTailscaleIps NOTIFY tailscaleIpsChanged)
    Q_PROPERTY(PeerStatus *self READ self BINDABLE bindableSelf NOTIFY selfChanged)
    Q_PROPERTY(ExitNodeStatus *exitNodeStatus READ exitNodeStatus BINDABLE bindableExitNodeStatus NOTIFY exitNodeStatusChanged)
    Q_PROPERTY(QStringList health READ health BINDABLE bindableHealth NOTIFY healthChanged)
    Q_PROPERTY(TailnetStatus *currentTailnet READ currentTailnet BINDABLE bindableCurrentTailnet NOTIFY currentTailnetChanged)
    Q_PROPERTY(PeerModel *peers READ peerModel CONSTANT)
    Q_PROPERTY(MullvadExitNodeModel *mullvadExitNodeModel READ mullvadExitNodeModel CONSTANT)
    Q_PROPERTY(SelfHostedExitNodeModel *selfHostedExitNodeModel READ selfHostedExitNodeModel CONSTANT)
    // QMap is not compatible with QBindable — BINDABLE is intentionally omitted here
    Q_PROPERTY(QMap<qint64, UserProfile *> users READ users NOTIFY usersChanged)
    Q_PROPERTY(ClientVersion *clientVersion READ clientVersion BINDABLE bindableClientVersion NOTIFY clientVersionChanged)
    Q_PROPERTY(QString suggestedExitNodeId READ suggestedExitNodeId BINDABLE bindableSuggestedExitNodeId NOTIFY suggestedExitNodeIdChanged)
    Q_PROPERTY(bool daemonRunning READ daemonRunning BINDABLE bindableDaemonRunning NOTIFY daemonRunningChanged)

private:
    // NOTE: signals are emitted manually *outside* the mutex lock in refresh() /
    // updateFromJson() to avoid deadlocks. Do NOT convert these to
    // Q_OBJECT_BINDABLE_PROPERTY, which would emit signals on assignment (inside
    // the lock).
    QMutex mMutex;

    QProperty<QString> mVersion;
    QProperty<bool> mIsTun;
    QProperty<BackendState> mBackendState;
    QProperty<bool> mHaveNodeKey;
    QProperty<QString> mAuthUrl;
    QProperty<QStringList> mTailscaleIps;
    QProperty<PeerStatus *> mSelf;
    QProperty<ExitNodeStatus *> mExitNodeStatus;
    QProperty<QStringList> mHealth;
    QProperty<TailnetStatus *> mCurrentTailnet;
    QMap<QString, PeerStatus *> mPeers;
    PeerModel *mPeerModel;
    QMap<qint64, UserProfile *> mUsers;
    QProperty<ClientVersion *> mClientVersion;
    QProperty<QString> mSuggestedExitNodeId;
    QProperty<bool> mDaemonRunning{true};

    MullvadExitNodeModel *mMullvadExitNodeModel;
    SelfHostedExitNodeModel *mSelfHostedExitNodeModel;

Q_SIGNALS:
    void versionChanged();
    void isTunChanged();
    void backendStateChanged();
    void haveNodeKeyChanged();
    void authUrlChanged();
    void tailscaleIpsChanged();
    void selfChanged();
    void exitNodeStatusChanged();
    void healthChanged();
    void currentTailnetChanged();
    void usersChanged();
    void clientVersionChanged();
    void suggestedExitNodeIdChanged();
    void daemonRunningChanged();

public:
    explicit Status(QObject *parent = nullptr);

    Q_INVOKABLE PeerStatus *peerWithId(const QString &peerId) const noexcept; // NOLINT(modernize-use-trailing-return-type)
    Q_INVOKABLE void refresh();
    void updateFromJson(QJsonObject &json);

    // Getters
    [[nodiscard]] auto version() const noexcept -> const QString &
    {
        return mVersion;
    }

    [[nodiscard]] auto isTun() const noexcept -> bool
    {
        return mIsTun;
    }

    [[nodiscard]] auto backendState() const noexcept -> BackendState
    {
        return mBackendState;
    }

    [[nodiscard]] auto haveNodeKey() const noexcept -> bool
    {
        return mHaveNodeKey;
    }

    [[nodiscard]] auto authUrl() const noexcept -> const QString &
    {
        return mAuthUrl;
    }

    [[nodiscard]] auto tailscaleIps() const noexcept -> const QStringList &
    {
        return mTailscaleIps;
    }

    [[nodiscard]] auto self() const noexcept -> PeerStatus *
    {
        return mSelf;
    }

    [[nodiscard]] auto exitNodeStatus() const noexcept -> ExitNodeStatus *
    {
        return mExitNodeStatus;
    }

    [[nodiscard]] auto health() const noexcept -> const QStringList &
    {
        return mHealth;
    }

    [[nodiscard]] auto currentTailnet() const noexcept -> TailnetStatus *
    {
        return mCurrentTailnet;
    }

    [[nodiscard]] auto peerModel() const noexcept -> PeerModel *
    {
        return mPeerModel;
    }

    [[nodiscard]] auto mullvadExitNodeModel() const noexcept -> MullvadExitNodeModel *
    {
        return mMullvadExitNodeModel;
    }

    [[nodiscard]] auto selfHostedExitNodeModel() const noexcept -> SelfHostedExitNodeModel *
    {
        return mSelfHostedExitNodeModel;
    }

    [[nodiscard]] auto users() const noexcept -> const QMap<qint64, UserProfile *> &
    {
        return mUsers;
    }

    [[nodiscard]] auto clientVersion() const noexcept -> ClientVersion *
    {
        return mClientVersion;
    }

    [[nodiscard]] auto suggestedExitNodeId() const noexcept -> const QString &
    {
        return mSuggestedExitNodeId;
    }

    [[nodiscard]] auto daemonRunning() const noexcept -> bool
    {
        return mDaemonRunning;
    }

    // Bindables
    [[nodiscard]] auto bindableVersion() -> QBindable<QString>
    {
        return {&mVersion};
    }

    [[nodiscard]] auto bindableIsTun() -> QBindable<bool>
    {
        return {&mIsTun};
    }

    [[nodiscard]] auto bindableBackendState() -> QBindable<BackendState>
    {
        return {&mBackendState};
    }

    [[nodiscard]] auto bindableHaveNodeKey() -> QBindable<bool>
    {
        return {&mHaveNodeKey};
    }

    [[nodiscard]] auto bindableAuthUrl() -> QBindable<QString>
    {
        return {&mAuthUrl};
    }

    [[nodiscard]] auto bindableTailscaleIps() -> QBindable<QStringList>
    {
        return {&mTailscaleIps};
    }

    [[nodiscard]] auto bindableSelf() -> QBindable<PeerStatus *>
    {
        return {&mSelf};
    }

    [[nodiscard]] auto bindableExitNodeStatus() -> QBindable<ExitNodeStatus *>
    {
        return {&mExitNodeStatus};
    }

    [[nodiscard]] auto bindableHealth() -> QBindable<QStringList>
    {
        return {&mHealth};
    }

    [[nodiscard]] auto bindableCurrentTailnet() -> QBindable<TailnetStatus *>
    {
        return {&mCurrentTailnet};
    }

    [[nodiscard]] auto bindableClientVersion() -> QBindable<ClientVersion *>
    {
        return {&mClientVersion};
    }

    [[nodiscard]] auto bindableSuggestedExitNodeId() -> QBindable<QString>
    {
        return {&mSuggestedExitNodeId};
    }

    [[nodiscard]] auto bindableDaemonRunning() -> QBindable<bool>
    {
        return {&mDaemonRunning};
    }
};

#endif // KTAILCTL_STATUS_HPP
