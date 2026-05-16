#ifndef KTAILCTL_PREFERENCES_PREFERENCES_HPP
#define KTAILCTL_PREFERENCES_PREFERENCES_HPP

#include "app_connector.hpp"
#include "auto_update.hpp"
#include "ktailctl_config.h"
#include "libktailctl_wrapper.h"
#include "logging_tailscale_preferences.hpp"
#include <QJsonObject>
#include <QMutex>
#include <QObject>
#include <QObjectBindableProperty>
#include <QProperty>
#include <QString>
#include <QTimer>
#include <QVariant>
#include <QtQmlIntegration/qqmlintegration.h>

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

public Q_SLOTS:
    Q_INVOKABLE void refresh();

public:
    explicit Preferences(QObject *parent = nullptr)
        : QObject(parent)
        , mAutoUpdate(new AutoUpdatePreferences(this))
        , mAppConnectorPreferences(new AppConnectorPreferences(this))
    {
    }

    void updateFromJson(QJsonObject &json);

    [[nodiscard]] const QString &controlUrl() const noexcept
    {
        return mControlUrl;
    }

    [[nodiscard]] bool routeAll() const noexcept
    {
        return mRouteAll;
    }

    [[nodiscard]] const QString &exitNodeId() const noexcept
    {
        return mExitNodeId;
    }

    [[nodiscard]] const QString &autoExitNode() const noexcept
    {
        return mAutoExitNode;
    }

    [[nodiscard]] const QString &lastUsedExitNode() const noexcept
    {
        return mLastUsedExitNode;
    }

    [[nodiscard]] bool exitNodeAllowLanAccess() const noexcept
    {
        return mExitNodeAllowLanAccess;
    }

    [[nodiscard]] bool corpDns() const noexcept
    {
        return mCorpDns;
    }

    [[nodiscard]] bool runSSH() const noexcept
    {
        return mRunSSH;
    }

    [[nodiscard]] bool runWebClient() const noexcept
    {
        return mRunWebClient;
    }

    [[nodiscard]] bool wantRunning() const noexcept
    {
        return mWantRunning;
    }

    [[nodiscard]] bool loggedOut() const noexcept
    {
        return mLoggedOut;
    }

    [[nodiscard]] bool shieldsUp() const noexcept
    {
        return mShieldsUp;
    }

    [[nodiscard]] const QStringList &advertiseTags() const noexcept
    {
        return mAdvertiseTags;
    }

    [[nodiscard]] const QString &hostname() const noexcept
    {
        return mHostname;
    }

    [[nodiscard]] bool notepadUrls() const noexcept
    {
        return mNotepadUrls;
    }

    [[nodiscard]] bool forceDaemon() const noexcept
    {
        return mForceDaemon;
    }

    [[nodiscard]] bool egg() const noexcept
    {
        return mEgg;
    }

    [[nodiscard]] const QStringList &advertiseRoutes() const noexcept
    {
        return mAdvertiseRoutes;
    }

    [[nodiscard]] const QStringList &advertiseServices() const noexcept
    {
        return mAdvertiseServices;
    }

    [[nodiscard]] bool sync() const noexcept
    {
        return mSync;
    }

    [[nodiscard]] bool noSnat() const noexcept
    {
        return mNoSnat;
    }

    [[nodiscard]] bool noStatefulFiltering() const noexcept
    {
        return mNoStatefulFiltering;
    }

    [[nodiscard]] NetfilterMode netfilterMode() const noexcept
    {
        return mNetfilterMode;
    }

    [[nodiscard]] const QString &operatorUser() const noexcept
    {
        return mOperatorUser;
    }

    [[nodiscard]] const QString &profileName() const noexcept
    {
        return mProfileName;
    }

    [[nodiscard]] AutoUpdatePreferences *autoUpdate() const noexcept
    {
        return mAutoUpdate;
    }

    [[nodiscard]] AppConnectorPreferences *appConnectorPreferences() const noexcept
    {
        return mAppConnectorPreferences;
    }

    [[nodiscard]] bool postureChecking() const noexcept
    {
        return mPostureChecking;
    }

    [[nodiscard]] const QString &netFilterKind() const noexcept
    {
        return mNetFilterKind;
    }

    [[nodiscard]] quint16 relayServerPort() const noexcept
    {
        return mRelayServerPort;
    }

    [[nodiscard]] const QStringList &relayServerStaticEndpoints() const noexcept
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

    [[nodiscard]] QBindable<QString> bindableControlUrl()
    {
        return {&mControlUrl};
    }

    [[nodiscard]] QBindable<bool> bindableRouteAll()
    {
        return {&mRouteAll};
    }

    [[nodiscard]] QBindable<QString> bindableExitNodeId()
    {
        return {&mExitNodeId};
    }

    [[nodiscard]] QBindable<QString> bindableAutoExitNode()
    {
        return {&mAutoExitNode};
    }

    [[nodiscard]] QBindable<QString> bindableLastUsedExitNode()
    {
        return {&mLastUsedExitNode};
    }

    [[nodiscard]] QBindable<bool> bindableExitNodeAllowLanAccess()
    {
        return {&mExitNodeAllowLanAccess};
    }

    [[nodiscard]] QBindable<bool> bindableCorpDns()
    {
        return {&mCorpDns};
    }

    [[nodiscard]] QBindable<bool> bindableRunSSH()
    {
        return {&mRunSSH};
    }

    [[nodiscard]] QBindable<bool> bindableRunWebClient()
    {
        return {&mRunWebClient};
    }

    [[nodiscard]] QBindable<bool> bindableWantRunning()
    {
        return {&mWantRunning};
    }

    [[nodiscard]] QBindable<bool> bindableLoggedOut()
    {
        return {&mLoggedOut};
    }

    [[nodiscard]] QBindable<bool> bindableShieldsUp()
    {
        return {&mShieldsUp};
    }

    [[nodiscard]] QBindable<QStringList> bindableAdvertiseTags()
    {
        return {&mAdvertiseTags};
    }

    [[nodiscard]] QBindable<QString> bindableHostname()
    {
        return {&mHostname};
    }

    [[nodiscard]] QBindable<bool> bindableNotepadUrls()
    {
        return {&mNotepadUrls};
    }

    [[nodiscard]] QBindable<bool> bindableForceDaemon()
    {
        return {&mForceDaemon};
    }

    [[nodiscard]] QBindable<bool> bindableEgg()
    {
        return {&mEgg};
    }

    [[nodiscard]] QBindable<QStringList> bindableAdvertiseRoutes()
    {
        return {&mAdvertiseRoutes};
    }

    [[nodiscard]] QBindable<QStringList> bindableAdvertiseServices()
    {
        return {&mAdvertiseServices};
    }

    [[nodiscard]] QBindable<bool> bindableSync()
    {
        return {&mSync};
    }

    [[nodiscard]] QBindable<bool> bindableNoSnat()
    {
        return {&mNoSnat};
    }

    [[nodiscard]] QBindable<bool> bindableNoStatefulFiltering()
    {
        return {&mNoStatefulFiltering};
    }

    [[nodiscard]] QBindable<NetfilterMode> bindableNetfilterMode()
    {
        return {&mNetfilterMode};
    }

    [[nodiscard]] QBindable<QString> bindableOperatorUser()
    {
        return {&mOperatorUser};
    }

    [[nodiscard]] QBindable<QString> bindableProfileName()
    {
        return {&mProfileName};
    }

    [[nodiscard]] QBindable<bool> bindablePostureChecking()
    {
        return {&mPostureChecking};
    }

    [[nodiscard]] QBindable<QString> bindableNetFilterKind()
    {
        return {&mNetFilterKind};
    }

    [[nodiscard]] QBindable<quint16> bindableRelayServerPort()
    {
        return {&mRelayServerPort};
    }

    [[nodiscard]] QBindable<QStringList> bindableRelayServerStaticEndpoints()
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
