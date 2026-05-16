#ifndef KTAILCTL_PREFERENCES_PREFERENCES_HPP
#define KTAILCTL_PREFERENCES_PREFERENCES_HPP

#include <QJsonObject>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QProperty>
#include <QString>
#include <QStringList>
#include <QStringLiteral>
#include <QVariant>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <stdint.h>

#include "app_connector.hpp"
#include "auto_update.hpp"

// https://pkg.go.dev/tailscale.com/ipn#Prefs
class Preferences : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum class NetfilterMode : uint8_t {
        Off = 0,
        NoDivert = 1,
        On = 2
    };

    Q_ENUM(NetfilterMode)

    Q_PROPERTY(QString controlUrl READ controlUrl BINDABLE bindableControlUrl NOTIFY controlUrlChanged)
    Q_PROPERTY(bool routeAll READ routeAll BINDABLE bindableRouteAll NOTIFY routeAllChanged)
    Q_PROPERTY(QString exitNodeId READ exitNodeId WRITE setExitNodeID BINDABLE bindableExitNodeId NOTIFY exitNodeIdChanged)
    Q_PROPERTY(QString autoExitNode READ autoExitNode BINDABLE bindableAutoExitNode NOTIFY autoExitNodeChanged)
    Q_PROPERTY(QString lastUsedExitNode READ lastUsedExitNode BINDABLE bindableLastUsedExitNode NOTIFY lastUsedExitNodeChanged)
    Q_PROPERTY(bool exitNodeAllowLanAccess READ exitNodeAllowLanAccess WRITE setExitNodeAllowLanAccess BINDABLE bindableExitNodeAllowLanAccess NOTIFY
                   exitNodeAllowLanAccessChanged)
    Q_PROPERTY(bool corpDns READ corpDns WRITE setCorpDns BINDABLE bindableCorpDns NOTIFY corpDnsChanged)
    Q_PROPERTY(bool runSSH READ runSSH WRITE setRunSSH BINDABLE bindableRunSSH NOTIFY runSSHChanged)
    Q_PROPERTY(bool runWebClient READ runWebClient BINDABLE bindableRunWebClient NOTIFY runWebClientChanged)
    Q_PROPERTY(bool wantRunning READ wantRunning BINDABLE bindableWantRunning NOTIFY wantRunningChanged)
    Q_PROPERTY(bool loggedOut READ loggedOut BINDABLE bindableLoggedOut NOTIFY loggedOutChanged)
    Q_PROPERTY(bool shieldsUp READ shieldsUp WRITE setShieldsUp BINDABLE bindableShieldsUp NOTIFY shieldsUpChanged)
    Q_PROPERTY(QStringList advertiseTags READ advertiseTags BINDABLE bindableAdvertiseTags NOTIFY advertiseTagsChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname BINDABLE bindableHostname NOTIFY hostnameChanged)
    Q_PROPERTY(bool notepadUrls READ notepadUrls BINDABLE bindableNotepadUrls NOTIFY notepadUrlsChanged)
    Q_PROPERTY(bool forceDaemon READ forceDaemon BINDABLE bindableForceDaemon NOTIFY forceDaemonChanged)
    Q_PROPERTY(bool egg READ egg BINDABLE bindableEgg NOTIFY eggChanged)
    Q_PROPERTY(QStringList advertiseRoutes READ advertiseRoutes BINDABLE bindableAdvertiseRoutes NOTIFY advertiseRoutesChanged)
    Q_PROPERTY(QStringList advertiseServices READ advertiseServices BINDABLE bindableAdvertiseServices NOTIFY advertiseServicesChanged)
    Q_PROPERTY(bool noStatefulFiltering READ noStatefulFiltering BINDABLE bindableNoStatefulFiltering NOTIFY noStatefulFilteringChanged)
    Q_PROPERTY(NetfilterMode netfilterMode READ netfilterMode BINDABLE bindableNetfilterMode NOTIFY netfilterModeChanged)
    Q_PROPERTY(QString operatorUser READ operatorUser BINDABLE bindableOperatorUser NOTIFY operatorUserChanged)
    Q_PROPERTY(QString profileName READ profileName WRITE setProfileName BINDABLE bindableProfileName NOTIFY profileNameChanged)
    Q_PROPERTY(AutoUpdatePreferences *autoUpdate READ autoUpdate CONSTANT)
    Q_PROPERTY(AppConnectorPreferences *appConnectorPreferences READ appConnectorPreferences CONSTANT)
    Q_PROPERTY(bool postureChecking READ postureChecking BINDABLE bindablePostureChecking NOTIFY postureCheckingChanged)
    Q_PROPERTY(QString netFilterKind READ netFilterKind BINDABLE bindableNetFilterKind NOTIFY netFilterKindChanged)
    Q_PROPERTY(quint16 relayServerPort READ relayServerPort BINDABLE bindableRelayServerPort NOTIFY relayServerPortChanged)
    Q_PROPERTY(QStringList relayServerStaticEndpoints READ relayServerStaticEndpoints BINDABLE bindableRelayServerStaticEndpoints NOTIFY
                   relayServerStaticEndpointsChanged)

    Q_INVOKABLE void refresh();

    explicit Preferences(QObject *parent = nullptr)
        : QObject(parent)
        , mAutoUpdate(new AutoUpdatePreferences(this))
        , mAppConnectorPreferences(new AppConnectorPreferences(this))
    {
    }

    void updateFromJson(QJsonObject &json);

    [[nodiscard]] auto controlUrl() const noexcept -> const QString &
    {
        return mControlUrl;
    }

    [[nodiscard]] auto routeAll() const noexcept -> bool
    {
        return mRouteAll;
    }

    [[nodiscard]] auto exitNodeId() const noexcept -> const QString &
    {
        return mExitNodeId;
    }

    [[nodiscard]] auto autoExitNode() const noexcept -> const QString &
    {
        return mAutoExitNode;
    }

    [[nodiscard]] auto lastUsedExitNode() const noexcept -> const QString &
    {
        return mLastUsedExitNode;
    }

    [[nodiscard]] auto exitNodeAllowLanAccess() const noexcept -> bool
    {
        return mExitNodeAllowLanAccess;
    }

    [[nodiscard]] auto corpDns() const noexcept -> bool
    {
        return mCorpDns;
    }

    [[nodiscard]] auto runSSH() const noexcept -> bool
    {
        return mRunSSH;
    }

    [[nodiscard]] auto runWebClient() const noexcept -> bool
    {
        return mRunWebClient;
    }

    [[nodiscard]] auto wantRunning() const noexcept -> bool
    {
        return mWantRunning;
    }

    [[nodiscard]] auto loggedOut() const noexcept -> bool
    {
        return mLoggedOut;
    }

    [[nodiscard]] auto shieldsUp() const noexcept -> bool
    {
        return mShieldsUp;
    }

    [[nodiscard]] auto advertiseTags() const noexcept -> const QStringList &
    {
        return mAdvertiseTags;
    }

    [[nodiscard]] auto hostname() const noexcept -> const QString &
    {
        return mHostname;
    }

    [[nodiscard]] auto notepadUrls() const noexcept -> bool
    {
        return mNotepadUrls;
    }

    [[nodiscard]] auto forceDaemon() const noexcept -> bool
    {
        return mForceDaemon;
    }

    [[nodiscard]] auto egg() const noexcept -> bool
    {
        return mEgg;
    }

    [[nodiscard]] auto advertiseRoutes() const noexcept -> const QStringList &
    {
        return mAdvertiseRoutes;
    }

    [[nodiscard]] auto advertiseServices() const noexcept -> const QStringList &
    {
        return mAdvertiseServices;
    }

    [[nodiscard]] auto sync() const noexcept -> bool
    {
        return mSync;
    }

    [[nodiscard]] auto noSnat() const noexcept -> bool
    {
        return mNoSnat;
    }

    [[nodiscard]] auto noStatefulFiltering() const noexcept -> bool
    {
        return mNoStatefulFiltering;
    }

    [[nodiscard]] auto netfilterMode() const noexcept -> NetfilterMode
    {
        return mNetfilterMode;
    }

    [[nodiscard]] auto operatorUser() const noexcept -> const QString &
    {
        return mOperatorUser;
    }

    [[nodiscard]] auto profileName() const noexcept -> const QString &
    {
        return mProfileName;
    }

    [[nodiscard]] auto autoUpdate() const noexcept -> AutoUpdatePreferences *
    {
        return mAutoUpdate;
    }

    [[nodiscard]] auto appConnectorPreferences() const noexcept -> AppConnectorPreferences *
    {
        return mAppConnectorPreferences;
    }

    [[nodiscard]] auto postureChecking() const noexcept -> bool
    {
        return mPostureChecking;
    }

    [[nodiscard]] auto netFilterKind() const noexcept -> const QString &
    {
        return mNetFilterKind;
    }

    [[nodiscard]] auto relayServerPort() const noexcept -> quint16
    {
        return mRelayServerPort;
    }

    [[nodiscard]] auto relayServerStaticEndpoints() const noexcept -> const QStringList &
    {
        return mRelayServerStaticEndpoints;
    }

    void setExitNodeID(const QString &exitNodeId);

    void setExitNodeAllowLanAccess(const bool value) const
    {
        _set_preference(QStringLiteral("ExitNodeAllowLanAccess"), value);
    }

    void setShieldsUp(const bool value) const
    {
        _set_preference(QStringLiteral("ShieldsUp"), value);
    }

    void setCorpDns(const bool value) const
    {
        _set_preference(QStringLiteral("CorpDNS"), value);
    }

    void setRunSSH(const bool value) const
    {
        _set_preference(QStringLiteral("RunSSH"), value);
    }

    void setHostname(const QString &value) const
    {
        _set_preference(QStringLiteral("Hostname"), value);
    }

    void setProfileName(const QString &value) const
    {
        _set_preference(QStringLiteral("ProfileName"), value);
    }

    [[nodiscard]] auto bindableControlUrl() -> QBindable<QString>
    {
        return {&mControlUrl};
    }

    [[nodiscard]] auto bindableRouteAll() -> QBindable<bool>
    {
        return {&mRouteAll};
    }

    [[nodiscard]] auto bindableExitNodeId() -> QBindable<QString>
    {
        return {&mExitNodeId};
    }

    [[nodiscard]] auto bindableAutoExitNode() -> QBindable<QString>
    {
        return {&mAutoExitNode};
    }

    [[nodiscard]] auto bindableLastUsedExitNode() -> QBindable<QString>
    {
        return {&mLastUsedExitNode};
    }

    [[nodiscard]] auto bindableExitNodeAllowLanAccess() -> QBindable<bool>
    {
        return {&mExitNodeAllowLanAccess};
    }

    [[nodiscard]] auto bindableCorpDns() -> QBindable<bool>
    {
        return {&mCorpDns};
    }

    [[nodiscard]] auto bindableRunSSH() -> QBindable<bool>
    {
        return {&mRunSSH};
    }

    [[nodiscard]] auto bindableRunWebClient() -> QBindable<bool>
    {
        return {&mRunWebClient};
    }

    [[nodiscard]] auto bindableWantRunning() -> QBindable<bool>
    {
        return {&mWantRunning};
    }

    [[nodiscard]] auto bindableLoggedOut() -> QBindable<bool>
    {
        return {&mLoggedOut};
    }

    [[nodiscard]] auto bindableShieldsUp() -> QBindable<bool>
    {
        return {&mShieldsUp};
    }

    [[nodiscard]] auto bindableAdvertiseTags() -> QBindable<QStringList>
    {
        return {&mAdvertiseTags};
    }

    [[nodiscard]] auto bindableHostname() -> QBindable<QString>
    {
        return {&mHostname};
    }

    [[nodiscard]] auto bindableNotepadUrls() -> QBindable<bool>
    {
        return {&mNotepadUrls};
    }

    [[nodiscard]] auto bindableForceDaemon() -> QBindable<bool>
    {
        return {&mForceDaemon};
    }

    [[nodiscard]] auto bindableEgg() -> QBindable<bool>
    {
        return {&mEgg};
    }

    [[nodiscard]] auto bindableAdvertiseRoutes() -> QBindable<QStringList>
    {
        return {&mAdvertiseRoutes};
    }

    [[nodiscard]] auto bindableAdvertiseServices() -> QBindable<QStringList>
    {
        return {&mAdvertiseServices};
    }

    [[nodiscard]] auto bindableSync() -> QBindable<bool>
    {
        return {&mSync};
    }

    [[nodiscard]] auto bindableNoSnat() -> QBindable<bool>
    {
        return {&mNoSnat};
    }

    [[nodiscard]] auto bindableNoStatefulFiltering() -> QBindable<bool>
    {
        return {&mNoStatefulFiltering};
    }

    [[nodiscard]] auto bindableNetfilterMode() -> QBindable<NetfilterMode>
    {
        return {&mNetfilterMode};
    }

    [[nodiscard]] auto bindableOperatorUser() -> QBindable<QString>
    {
        return {&mOperatorUser};
    }

    [[nodiscard]] auto bindableProfileName() -> QBindable<QString>
    {
        return {&mProfileName};
    }

    [[nodiscard]] auto bindablePostureChecking() -> QBindable<bool>
    {
        return {&mPostureChecking};
    }

    [[nodiscard]] auto bindableNetFilterKind() -> QBindable<QString>
    {
        return {&mNetFilterKind};
    }

    [[nodiscard]] auto bindableRelayServerPort() -> QBindable<quint16>
    {
        return {&mRelayServerPort};
    }

    [[nodiscard]] auto bindableRelayServerStaticEndpoints() -> QBindable<QStringList>
    {
        return {&mRelayServerStaticEndpoints};
    }

Q_SIGNALS:
    void controlUrlChanged();
    void routeAllChanged();
    void exitNodeIdChanged();
    void autoExitNodeChanged();
    void lastUsedExitNodeChanged();
    void exitNodeAllowLanAccessChanged();
    void corpDnsChanged();
    void runSSHChanged();
    void runWebClientChanged();
    void wantRunningChanged();
    void loggedOutChanged();
    void shieldsUpChanged();
    void advertiseTagsChanged();
    void hostnameChanged();
    void notepadUrlsChanged();
    void forceDaemonChanged();
    void eggChanged();
    void advertiseRoutesChanged();
    void advertiseServicesChanged();
    void noStatefulFilteringChanged();
    void netfilterModeChanged();
    void operatorUserChanged();
    void profileNameChanged();
    void postureCheckingChanged();
    void netFilterKindChanged();
    void relayServerPortChanged();
    void relayServerStaticEndpointsChanged();

private:
    QMutex mMutex;

    QProperty<bool> mSync;
    QProperty<bool> mNoSnat;

    AutoUpdatePreferences *mAutoUpdate;
    AppConnectorPreferences *mAppConnectorPreferences;

    void _set_preference(const QString &key, const QVariant &value) const;

    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QString, mControlUrl, &Preferences::controlUrlChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mRouteAll, &Preferences::routeAllChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QString, mExitNodeId, &Preferences::exitNodeIdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QString, mAutoExitNode, &Preferences::autoExitNodeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QString, mLastUsedExitNode, &Preferences::lastUsedExitNodeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mExitNodeAllowLanAccess, &Preferences::exitNodeAllowLanAccessChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mCorpDns, &Preferences::corpDnsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mRunSSH, &Preferences::runSSHChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mRunWebClient, &Preferences::runWebClientChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mWantRunning, &Preferences::wantRunningChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mLoggedOut, &Preferences::loggedOutChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mShieldsUp, &Preferences::shieldsUpChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QStringList, mAdvertiseTags, &Preferences::advertiseTagsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QString, mHostname, &Preferences::hostnameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mNotepadUrls, &Preferences::notepadUrlsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mForceDaemon, &Preferences::forceDaemonChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mEgg, &Preferences::eggChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QStringList, mAdvertiseRoutes, &Preferences::advertiseRoutesChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QStringList, mAdvertiseServices, &Preferences::advertiseServicesChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mNoStatefulFiltering, &Preferences::noStatefulFilteringChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, NetfilterMode, mNetfilterMode, &Preferences::netfilterModeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QString, mOperatorUser, &Preferences::operatorUserChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QString, mProfileName, &Preferences::profileNameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, bool, mPostureChecking, &Preferences::postureCheckingChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QString, mNetFilterKind, &Preferences::netFilterKindChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, quint16, mRelayServerPort, &Preferences::relayServerPortChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Preferences, QStringList, mRelayServerStaticEndpoints, &Preferences::relayServerStaticEndpointsChanged)
};

#endif // KTAILCTL_PREFERENCES_PREFERENCES_HPP
