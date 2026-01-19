#ifndef KTAILCTL_PREFERENCES_PREFERENCES_HPP
#define KTAILCTL_PREFERENCES_PREFERENCES_HPP

#include "app_connector.hpp"
#include "auto_update.hpp"
#include "libktailctl_wrapper.h"
#include "logging_tailscale_preferences.hpp"

#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QProperty>
#include <QString>

// https://pkg.go.dev/tailscale.com/ipn#Prefs
class Preferences : public QObject
{
    Q_OBJECT

public:
    enum class NetfilterMode : uint8_t { Off = 0, NoDivert = 1, On = 2 };
    Q_ENUM(NetfilterMode)

    Q_PROPERTY(QString controlUrl READ controlUrl BINDABLE bindableControlUrl)
    Q_PROPERTY(bool routeAll READ routeAll BINDABLE bindableRouteAll)
    Q_PROPERTY(QString exitNodeId READ exitNodeId BINDABLE bindableExitNodeId)
    Q_PROPERTY(QString autoExitNode READ autoExitNode BINDABLE bindableAutoExitNode)
    Q_PROPERTY(QString lastUsedExitNode READ lastUsedExitNode BINDABLE bindableLastUsedExitNode)
    Q_PROPERTY(bool exitNodeAllowLanAccesss READ exitNodeAllowLanAccess WRITE setExitNodeAllowLanAccess BINDABLE bindableExitNodeAllowLanAccess)
    Q_PROPERTY(bool corpDns READ corpDns WRITE setCorpDns BINDABLE bindableCorpDns)
    Q_PROPERTY(bool runSSH READ runSSH WRITE setRunSSH BINDABLE bindableRunSSH)
    Q_PROPERTY(bool runWebClient READ runWebClient BINDABLE bindableRunWebClient)
    Q_PROPERTY(bool wantRunning READ wantRunning BINDABLE bindableWantRunning)
    Q_PROPERTY(bool loggedOut READ loggedOut BINDABLE bindableLoggedOut)
    Q_PROPERTY(bool shieldsUp READ shieldsUp BINDABLE bindableShieldsUp)
    Q_PROPERTY(QStringList advertiseTags READ advertiseTags BINDABLE bindableAdvertiseTags)
    Q_PROPERTY(QString hostname READ hostname BINDABLE bindableHostname)
    Q_PROPERTY(bool notepadUrls READ notepadUrls BINDABLE bindableNotepadUrls)
    Q_PROPERTY(bool forceDaemon READ forceDaemon BINDABLE bindableForceDaemon)
    Q_PROPERTY(bool egg READ egg BINDABLE bindableEgg)
    Q_PROPERTY(QStringList advertiseRoutes READ advertiseRoutes BINDABLE bindableAdvertiseRoutes)
    Q_PROPERTY(QStringList advertiseServices READ advertiseServices BINDABLE bindableAdvertiseServices)
    Q_PROPERTY(bool noStatefulFiltering READ noStatefulFiltering BINDABLE bindableNoStatefulFiltering)
    Q_PROPERTY(NetfilterMode netfilterMode READ netfilterMode BINDABLE bindableNetfilterMode)
    Q_PROPERTY(QString operatorUser READ operatorUser BINDABLE bindableOperatorUser)
    Q_PROPERTY(QString profileName READ profileName BINDABLE bindableProfileName)
    Q_PROPERTY(AutoUpdatePreferences *autoUpdate READ autoUpdate CONSTANT)
    Q_PROPERTY(AppConnectorPreferences *appConnectorPreferences READ appConnectorPreferences CONSTANT)
    Q_PROPERTY(bool postureChecking READ postureChecking BINDABLE bindablePostureChecking)
    Q_PROPERTY(QString netFilterKind READ netFilterKind BINDABLE bindableNetFilterKind)
    Q_PROPERTY(quint16 relayServerPort READ relayServerPort BINDABLE bindableRelayServerPort)
    Q_PROPERTY(QStringList relayServerStaticEndpoints READ relayServerStaticEndpoints BINDABLE bindableRelayServerStaticEndpoints)

private:
    QMutex mMutex;

    QProperty<QString> mControlUrl;
    QProperty<bool> mRouteAll;
    QProperty<QString> mExitNodeId;
    // exit node IP
    QProperty<QString> mAutoExitNode;
    QProperty<QString> mLastUsedExitNode;
    QProperty<bool> mExitNodeAllowLanAccess;
    QProperty<bool> mCorpDns;
    QProperty<bool> mRunSSH;
    QProperty<bool> mRunWebClient;
    QProperty<bool> mWantRunning;
    QProperty<bool> mLoggedOut;
    QProperty<bool> mShieldsUp;
    QProperty<QStringList> mAdvertiseTags;
    QProperty<QString> mHostname;
    QProperty<bool> mNotepadUrls;
    QProperty<bool> mForceDaemon;
    QProperty<bool> mEgg;
    QProperty<QStringList> mAdvertiseRoutes;
    QProperty<QStringList> mAdvertiseServices;
    QProperty<bool> mSync;
    QProperty<bool> mNoSnat;
    QProperty<bool> mNoStatefulFiltering;
    QProperty<NetfilterMode> mNetfilterMode;
    QProperty<QString> mOperatorUser;
    QProperty<QString> mProfileName;
    AutoUpdatePreferences *mAutoUpdate;
    AppConnectorPreferences *mAppConnectorPreferences;
    QProperty<bool> mPostureChecking;
    QProperty<QString> mNetFilterKind;
    // drive shares
    QProperty<quint16> mRelayServerPort;
    QProperty<QStringList> mRelayServerStaticEndpoints;
    // persist stuff?

    void _set_preference(const QString& key, const QVariant& value)
    {
        GoString string;
        const QByteArray json = QJsonDocument::fromVariant(QVariantMap{{key, value}}).toJson();
        string.n = json.size();
        string.p = json.data();
        if (tailscale_set_preferences(&string) == 0) {
            qCCritical(Logging::Tailscale::Preferences) << "Failed to set preferences!";
        } else {
            // trigger a refresh of the settings maybe after a short delay
        }
    }

public:
    explicit Preferences(QObject *parent = nullptr)
        : QObject(parent)
        , mAutoUpdate(new AutoUpdatePreferences(this))
        , mAppConnectorPreferences(new AppConnectorPreferences(this))
    {
    }

    Q_INVOKABLE void refresh()
    {
        QMutexLocker lock(&mMutex);

        const std::unique_ptr<char, decltype(&::free)> json_str(tailscale_prefs(), &free);
        const QByteArray json_buffer(json_str.get(), ::strlen(json_str.get()));
        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
        if (error.error != QJsonParseError::NoError) {
            qCCritical(Logging::Tailscale::Preferences) << error.errorString();
            return;
        }
        QJsonObject json_obj = json.object();

        updateFromJson(json_obj);
        qCInfo(Logging::Tailscale::Preferences) << "Preferences refreshed";
    }

    void updateFromJson(QJsonObject &json)
    {
        mControlUrl = json.take(QStringLiteral("ControlURL")).toString();
        mRouteAll = json.take(QStringLiteral("RouteAll")).toBool();
        mExitNodeId = json.take(QStringLiteral("ExitNodeID")).toString();
        mAutoExitNode = json.take(QStringLiteral("AutoExitNode")).toString();
        mLastUsedExitNode = json.take(QStringLiteral("InternalExitNodePrior")).toString();
        mExitNodeAllowLanAccess = json.take(QStringLiteral("ExitNodeAllowLANAccess")).toBool();
        mCorpDns = json.take(QStringLiteral("CorpDns")).toBool();
        mRunSSH = json.take(QStringLiteral("RunSSH")).toBool();
        mRunWebClient = json.take(QStringLiteral("RunWebClient")).toBool();
        mWantRunning = json.take(QStringLiteral("WantRunning")).toBool();
        mLoggedOut = json.take(QStringLiteral("LoggedOut")).toBool();
        mShieldsUp = json.take(QStringLiteral("ShieldsUp")).toBool();
        mAdvertiseTags = json.take(QStringLiteral("AdvertiseTags")).toVariant().toStringList();
        mHostname = json.take(QStringLiteral("Hostname")).toString();
        mNotepadUrls = json.take(QStringLiteral("NotepadURLs")).toBool();
        mForceDaemon = json.take(QStringLiteral("ForceDaemon")).toBool();
        mEgg = json.take(QStringLiteral("Egg")).toBool();
        mAdvertiseRoutes = json.take(QStringLiteral("AdvertiseRoutes")).toVariant().toStringList();
        mAdvertiseServices = json.take(QStringLiteral("AdvertiseServices")).toVariant().toStringList();
        mSync = json.take(QStringLiteral("Sync")).toBool();
        mNoSnat = json.take(QStringLiteral("NoSNAT")).toBool();
        mNoStatefulFiltering = json.take(QStringLiteral("NoStatefulFiltering")).toBool();
        mNetfilterMode = static_cast<NetfilterMode>(json.take(QStringLiteral("NetfilterMode")).toInt());
        mOperatorUser = json.take(QStringLiteral("OperatorUser")).toString();
        mProfileName = json.take(QStringLiteral("ProfileName")).toString();
        if (json.contains(QStringLiteral("AutoUpdate"))) {
            auto autoUpdateJson = json.take(QStringLiteral("AutoUpdate")).toObject();
            mAutoUpdate->updateFromJson(autoUpdateJson);
        }
        if (json.contains(QStringLiteral("AppConnector"))) {
            auto appConnectorJson = json.take(QStringLiteral("AppConnector")).toObject();
            mAppConnectorPreferences->updateFromJson(appConnectorJson);
        }
        mPostureChecking = json.take(QStringLiteral("PostureChecking")).toBool();
        mNetFilterKind = json.take(QStringLiteral("NetFilterKind")).toString();
        mRelayServerPort = static_cast<quint16>(json.take(QStringLiteral("RelayServerPort")).toInteger());
        mRelayServerStaticEndpoints = json.take(QStringLiteral("RelayServerStaticEndpoints")).toVariant().toStringList();
    }

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

    void setExitNodeAllowLanAccess(bool value)
    {
        _set_preference(QStringLiteral("ExitNodeAllowLanAccess"), value);
    }

    void setCorpDns(bool value)
    {
        _set_preference(QStringLiteral("CorpDNS"), value);
    }

    void setRunSSH(bool value)
    {
        _set_preference(QStringLiteral("RunSSH"), value);
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
};

#endif // KTAILCTL_PREFERENCES_PREFERENCES_HPP
