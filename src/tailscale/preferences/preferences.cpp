#include "preferences.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMutexLocker>

void Preferences::_set_preference(const QString &key, const QVariant &value) const
{
    static constexpr int preferences_update_dealy_ms = 200;
    qCInfo(Logging::Tailscale::Preferences) << "Set preference " << key << " to " << value;
    GoString string;
    const QByteArray json = QJsonDocument::fromVariant(QVariantMap{{key, value}}).toJson();
    string.n = json.size();
    string.p = json.data();
    if (tailscale_set_preferences(&string) == 0) {
        qCCritical(Logging::Tailscale::Preferences) << "Failed to set preferences!";
    } else {
        QTimer::singleShot(preferences_update_dealy_ms, this, &Preferences::refresh);
    }
}

void Preferences::refresh()
{
    const QMutexLocker lock(&mMutex);

    const std::unique_ptr<char, decltype(&free)> json_str(tailscale_prefs(), &free);
    const QByteArray json_buffer(json_str.get(), strlen(json_str.get()));
    QJsonParseError error;
    const QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
    if (error.error != QJsonParseError::NoError) {
        qCCritical(Logging::Tailscale::Preferences) << error.errorString();
        return;
    }
    QJsonObject json_obj = json.object();

    updateFromJson(json_obj);
    qCInfo(Logging::Tailscale::Preferences) << "Preferences refreshed";
}

void Preferences::updateFromJson(QJsonObject &json)
{
    mControlUrl = json.take(QStringLiteral("ControlURL")).toString();
    mRouteAll = json.take(QStringLiteral("RouteAll")).toBool();
    mExitNodeId = json.take(QStringLiteral("ExitNodeID")).toString();
    mAutoExitNode = json.take(QStringLiteral("AutoExitNode")).toString();
    mLastUsedExitNode = json.take(QStringLiteral("InternalExitNodePrior")).toString();
    mExitNodeAllowLanAccess = json.take(QStringLiteral("ExitNodeAllowLANAccess")).toBool();
    mCorpDns = json.take(QStringLiteral("CorpDNS")).toBool();
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

void Preferences::setExitNodeID(const QString &exitNodeId)
{
    if (!exitNodeId.isEmpty()) {
        auto *config = Config::self();
        config->setLastUsedExitNode(exitNodeId);
        config->save();
    }
    _set_preference(QStringLiteral("ExitNodeID"), exitNodeId);
}
