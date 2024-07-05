#include "peer_data.hpp"
#include <iostream>

Q_LOGGING_CATEGORY(logcat_peer_data, "org.fkoehler.KTailctl.PeerData")

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
        j.at("OS").get_to<QString>(p.mOs);
        j.at("TailscaleIPs").get_to<QStringList>(p.mTailscaleIps);
        j.at("Relay").get_to<QString>(p.mRelay);
        j.at("RxBytes").get_to<long>(p.mRxBytes);
        j.at("TxBytes").get_to<long>(p.mTxBytes);
        j.at("Created").get_to<QString>(p.mCreated);
        j.at("LastSeen").get_to<QString>(p.mLastSeen);
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
        } else {
            p.mTags.clear();
        }
        p.mIsMullvad = p.mTags.contains("tag:mullvad-exit-node");
        if (j.contains("Location")) {
            const json &loc = j.at("Location");
            loc.at("Country").get_to<QString>(p.mCountry);
            loc.at("CountryCode").get_to<QString>(p.mCountryCode);
            loc.at("City").get_to<QString>(p.mCity);
            loc.at("CityCode").get_to<QString>(p.mCityCode);
        } else {
            p.mCountryCode = QStringLiteral("UNK");
            p.mCountry = QStringLiteral("UNKNOWN");
            p.mCityCode = QStringLiteral("UNK");
            p.mCity = QStringLiteral("UNKNOWN");
        }
    } catch (json::exception &e) {
        qCCritical(logcat_peer_data) << "Error parsing peer data: " << e.what();
    }
}
