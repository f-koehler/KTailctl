#include "peer_data.hpp"
#include "logging_peer_data.hpp"
#include <QRegularExpression>

bool PeerData::isRunningSSH() const
{
    return !mSshHostKeys.isEmpty();
}

QString PeerData::sshCommand() const
{
    if (!isRunningSSH()) {
        return {""};
    }
    return QString("tailscale ssh %1").arg(mDnsName);
}

void from_json(const json &j, QVector<PeerData> &peers)
{
    peers.resize(j.size());
    std::size_t i = 0;
    for (const auto &el : j.items()) {
        el.value().get_to<PeerData>(peers[i]);
        ++i;
    }
    std::stable_sort(peers.begin(), peers.end(), [](const PeerData &a, const PeerData &b) {
        return a.mDnsName < b.mDnsName;
    });
}

void from_json(const json &j, PeerData &p)
{
    try {
        j.at("ID").get_to<QString>(p.mId);
        j.at("PublicKey").get_to<QString>(p.mPublicKey);
        j.at("HostName").get_to<QString>(p.mHostName);
        j.at("DNSName").get_to<QString>(p.mDnsName);
        if (!p.mDnsName.isEmpty() && (p.mDnsName.back() == QChar('.'))) {
            p.mDnsName.chop(1);
        }
        j.at("OS").get_to<QString>(p.mOs);
        j.at("TailscaleIPs").get_to<QStringList>(p.mTailscaleIps);
        p.mAdminPanelUrl = p.mTailscaleIps.isEmpty() ? QStringLiteral("https://login.tailscale.com/admin/machines/")
                                                     : QString("https://login.tailscale.com/admin/machines/%1").arg(p.mTailscaleIps.first());
        j.at("Relay").get_to<QString>(p.mRelay);
        j.at("RxBytes").get_to<long>(p.mRxBytes);
        j.at("TxBytes").get_to<long>(p.mTxBytes);
        j.at("Created").get_to<QString>(p.mCreated);
        j.at("LastSeen").get_to<QString>(p.mLastSeen);
        if (p.mLastSeen == QStringLiteral("0001-01-01T00:00:00Z")) {
            p.mLastSeen = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
        }
        j.at("Online").get_to<bool>(p.mIsOnline);
        j.at("Active").get_to<bool>(p.mIsActive);
        j.at("ExitNode").get_to<bool>(p.mIsCurrentExitNode);
        j.at("ExitNodeOption").get_to<bool>(p.mIsExitNode);
        if (j.contains("sshHostKeys")) {
            j["sshHostKeys"].get_to<QStringList>(p.mSshHostKeys);
        } else {
            p.mSshHostKeys.clear();
        }
        if (j.contains("Tags")) {
            j["Tags"].get_to<QStringList>(p.mTags);
            p.mTags.replaceInStrings(QRegularExpression("^tag:"), "");
        } else {
            p.mTags.clear();
        }
        p.mIsMullvad = p.mTags.contains("mullvad-exit-node");
        if (j.contains("Location")) {
            const json &loc = j.at("Location");
            loc.at("Country").get_to<QString>(p.mCountry);
            loc.at("CountryCode").get_to<QString>(p.mCountryCode);
            loc.at("City").get_to<QString>(p.mCity);
            loc.at("CityCode").get_to<QString>(p.mCityCode);
        } else {
            p.mCountryCode.clear();
            p.mCountry.clear();
            p.mCityCode.clear();
            p.mCity.clear();
        }
    } catch (json::exception &e) {
        qCCritical(Logging::PeerData) << "Error parsing peer data: " << e.what();
    }
}
