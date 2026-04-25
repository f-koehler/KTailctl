#ifndef KTAILCTL_STATUS_HPP
#define KTAILCTL_STATUS_HPP

#include "logging_tailscale_status.hpp"

#include "client_version.hpp"
#include "exit_node_status.hpp"
#include "mullvad_exit_node_model.hpp"
#include "peer_status.hpp"
#include "property_list_model.hpp"
#include "self_hosted_exit_node_model.hpp"
#include "tailnet_status.hpp"
#include "user_profile.hpp"

#include <QJsonObject>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QString>

// https://pkg.go.dev/tailscale.com/ipn/ipnstate#Status
class Status : public QObject
{
    Q_OBJECT

public:
    using PeerModel = PropertyListModel<PeerStatus, PropertyListModelOwnership::External>;

    enum class BackendState : uint8_t { NoState, NeedsLogin, NeedsMachineAuth, Stopped, Starting, Running };

    Q_ENUM(BackendState)

    Q_PROPERTY(QString version READ version BINDABLE bindableVersion)
    Q_PROPERTY(bool isTun READ isTun BINDABLE bindableIsTun)
    Q_PROPERTY(BackendState backendState READ backendState BINDABLE bindableBackendState NOTIFY backendStateChanged)
    Q_PROPERTY(bool haveNodeKey READ haveNodeKey BINDABLE bindableHaveNodeKey)
    Q_PROPERTY(QString authUrl READ authUrl BINDABLE bindableAuthUrl)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps BINDABLE bindableTailscaleIps)
    Q_PROPERTY(PeerStatus *self READ self BINDABLE bindableSelf)
    Q_PROPERTY(ExitNodeStatus *exitNodeStatus READ exitNodeStatus BINDABLE bindableExitNodeStatus)
    Q_PROPERTY(QStringList health READ health BINDABLE bindableHealth)
    Q_PROPERTY(TailnetStatus *currentTailnet READ currentTailnet BINDABLE bindableCurrentTailnet)
    Q_PROPERTY(PeerModel *peers READ peerModel CONSTANT)
    Q_PROPERTY(MullvadExitNodeModel *mullvadExitNodeModel READ mullvadExitNodeModel CONSTANT)
    Q_PROPERTY(SelfHostedExitNodeModel *selfHostedExitNodeModel READ selfHostedExitNodeModel CONSTANT)
    Q_PROPERTY(QMap<qint64, UserProfile *> users READ users BINDABLE bindableUsers)
    Q_PROPERTY(ClientVersion *clientVersion READ clientVersion BINDABLE bindableClientVersion)
    Q_PROPERTY(QString suggestedExitNodeId READ suggestedExitNodeId BINDABLE bindableSuggestedExitNodeId)

private:
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
    QProperty<QMap<qint64, UserProfile *>> mUsers;
    QProperty<ClientVersion *> mClientVersion;
    QProperty<QString> mSuggestedExitNodeId;

    MullvadExitNodeModel *mMullvadExitNodeModel;
    SelfHostedExitNodeModel *mSelfHostedExitNodeModel;

Q_SIGNALS:
    void backendStateChanged();
    void exitNodeStatusChanged();

public:
    explicit Status(QObject *parent = nullptr);

    Q_INVOKABLE PeerStatus *peerWithId(const QString &peerId) const noexcept;
    Q_INVOKABLE void refresh();
    void updateFromJson(QJsonObject &json);

    // Getters
    [[nodiscard]] const QString &version() const noexcept
    {
        return mVersion;
    }

    [[nodiscard]] bool isTun() const noexcept
    {
        return mIsTun;
    }

    [[nodiscard]] BackendState backendState() const noexcept
    {
        return mBackendState;
    }

    [[nodiscard]] bool haveNodeKey() const noexcept
    {
        return mHaveNodeKey;
    }

    [[nodiscard]] const QString &authUrl() const noexcept
    {
        return mAuthUrl;
    }

    [[nodiscard]] const QStringList &tailscaleIps() const noexcept
    {
        return mTailscaleIps;
    }

    [[nodiscard]] PeerStatus *self() const noexcept
    {
        return mSelf;
    }

    [[nodiscard]] ExitNodeStatus *exitNodeStatus() const noexcept
    {
        return mExitNodeStatus;
    }

    [[nodiscard]] const QStringList &health() const noexcept
    {
        return mHealth;
    }

    [[nodiscard]] TailnetStatus *currentTailnet() const noexcept
    {
        return mCurrentTailnet;
    }

    [[nodiscard]] PeerModel *peerModel() const noexcept
    {
        return mPeerModel;
    }

    [[nodiscard]] MullvadExitNodeModel *mullvadExitNodeModel() const noexcept
    {
        return mMullvadExitNodeModel;
    }

    [[nodiscard]] SelfHostedExitNodeModel *selfHostedExitNodeModel() const noexcept
    {
        return mSelfHostedExitNodeModel;
    }

    [[nodiscard]] const QMap<qint64, UserProfile *> &users() const noexcept
    {
        return mUsers;
    }

    [[nodiscard]] ClientVersion *clientVersion() const noexcept
    {
        return mClientVersion;
    }

    [[nodiscard]] const QString &suggestedExitNodeId() const noexcept
    {
        return mSuggestedExitNodeId;
    }

    // Bindables
    [[nodiscard]] QBindable<QString> bindableVersion()
    {
        return {&mVersion};
    }

    [[nodiscard]] QBindable<bool> bindableIsTun()
    {
        return {&mIsTun};
    }

    [[nodiscard]] QBindable<BackendState> bindableBackendState()
    {
        return {&mBackendState};
    }

    [[nodiscard]] QBindable<bool> bindableHaveNodeKey()
    {
        return {&mHaveNodeKey};
    }

    [[nodiscard]] QBindable<QString> bindableAuthUrl()
    {
        return {&mAuthUrl};
    }

    [[nodiscard]] QBindable<QStringList> bindableTailscaleIps()
    {
        return {&mTailscaleIps};
    }

    [[nodiscard]] QBindable<PeerStatus *> bindableSelf()
    {
        return {&mSelf};
    }

    [[nodiscard]] QBindable<ExitNodeStatus *> bindableExitNodeStatus()
    {
        return {&mExitNodeStatus};
    }

    [[nodiscard]] QBindable<QStringList> bindableHealth()
    {
        return {&mHealth};
    }

    [[nodiscard]] QBindable<TailnetStatus *> bindableCurrentTailnet()
    {
        return {&mCurrentTailnet};
    }

    [[nodiscard]] QBindable<QMap<qint64, UserProfile *>> bindableUsers()
    {
        return {&mUsers};
    }

    [[nodiscard]] QBindable<ClientVersion *> bindableClientVersion()
    {
        return {&mClientVersion};
    }

    [[nodiscard]] QBindable<QString> bindableSuggestedExitNodeId()
    {
        return {&mSuggestedExitNodeId};
    }
};

#endif // KTAILCTL_STATUS_HPP
