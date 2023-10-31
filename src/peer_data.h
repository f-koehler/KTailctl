#ifndef KTAILCTL_PEER_DATA_H
#define KTAILCTL_PEER_DATA_H

#include "json.h"
#include "location_data.h"

struct PeerData {
    QString id;
    QString publicKey;
    QString hostName;
    QString dnsName;
    QString os;
    QStringList tailscaleIps;
    QString relay;
    long rxBytes;
    long txBytes;
    QString created;
    QString lastSeen;
    bool isOnline;
    bool isActive;
    bool isCurrentExitNode;
    bool isExitNode;
    QStringList sshHostKeys;
    bool isRunningSSH;
    QStringList tags;
    bool isMullvad;
    std::optional<LocationData> location;
};

void from_json(const json &j, PeerData &p);
void from_json(const json &j, QVector<PeerData> &peers);

#endif /* KTAILCTL_PEER_DATA_H */
