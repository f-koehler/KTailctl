#include "peer.hpp"

void Peer::fromJson(const QJsonObject &object)
{
    setId(object.value(QStringLiteral("ID")).toString());
    setPublicKey(object.value(QStringLiteral("PublicKey")).toString());
    setHostName(object.value("HostName").toString());
    setDnsName(object.value("DNSName").toString());
    setOs(object.value("OS").toString());

    setTailscaleIps(object.value("TailscaleIPs").toVariant().toStringList());

    setRelay(object.value("Relay").toString());
    setReceivedBytes(object.value("RxBytes").toInteger());
    setTransmittedBytes(object.value("TxBytes").toInteger());
    // TODO(fk): parse lastSeen and Created
    setIsOnline(object.value("IsOnline").toBool());
    setIsActive(object.value("IsActive").toBool());
    // TODO(fk): setIsCurrentExitNode
    // TODO(fk): setIsExitNode
}
