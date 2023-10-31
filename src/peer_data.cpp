#include "peer_data.h"

void from_json(const json &j, QVector<PeerData> &peers)
{
    peers.resize(j.size());
    std::size_t i = 0;
    for (const auto &el : j.items()) {
        el.value().get_to<PeerData>(peers[i]);
        ++i;
    }
    std::stable_sort(peers.begin(), peers.end(), [](const PeerData &a, const PeerData &b) {
        return a.dnsName < b.dnsName;
    });
}

void from_json(const json &j, PeerData &p)
{
    try {
        j.at("ID").get_to<QString>(p.id);
        j.at("PublicKey").get_to<QString>(p.publicKey);
        j.at("HostName").get_to<QString>(p.hostName);
        j.at("DNSName").get_to<QString>(p.dnsName);
        j.at("OS").get_to<QString>(p.os);
        j.at("TailscaleIPs").get_to<QStringList>(p.tailscaleIps);
        j.at("Relay").get_to<QString>(p.relay);
        j.at("RxBytes").get_to<long>(p.rxBytes);
        j.at("TxBytes").get_to<long>(p.txBytes);
        j.at("Created").get_to<QString>(p.created);
        j.at("LastSeen").get_to<QString>(p.lastSeen);
        j.at("Online").get_to<bool>(p.isOnline);
        j.at("Active").get_to<bool>(p.isActive);
        j.at("ExitNode").get_to<bool>(p.isCurrentExitNode);
        j.at("ExitNodeOption").get_to<bool>(p.isExitNode);
        if (j.contains("sshHostKeys")) {
            j["sshHostKeys"].get_to<QStringList>(p.sshHostKeys);
        } else {
            p.sshHostKeys.clear();
        }
        if (j.contains("Tags")) {
            j["Tags"].get_to<QStringList>(p.tags);
        } else {
            p.tags.clear();
        }
        p.isMullvad = p.tags.contains("tag:mullvad-exit-node");
        if (j.contains("Location")) {
            p.location = j["Location"].get<LocationData>();
        }
    } catch (json::exception &e) {
        qCritical() << "Error parsing peer data: " << e.what();
    }
}
