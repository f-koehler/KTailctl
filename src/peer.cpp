#include "peer.h"

static constexpr const char *TAILSCALE_JSON_NOWDATE = "0001-01-01T00:00:00Z";
QDateTime get_date_from_string(const QString &string)
{
    if (string == TAILSCALE_JSON_NOWDATE) {
        return QDateTime::currentDateTime();
    } else {
        return QDateTime::fromString(string, Qt::ISODateWithMs);
    }
}

Peer::Peer(QObject *parent)
    : QObject(parent)
{
}

const QString &Peer::id() const
{
    return mData.id;
}
const QString &Peer::publicKey() const
{
    return mData.publicKey;
}
const QString &Peer::hostName() const
{
    return mData.hostName;
}
const QString &Peer::dnsName() const
{
    return mData.dnsName;
}
const QString &Peer::os() const
{
    return mData.os;
}
const QStringList &Peer::tailscaleIps() const
{
    return mData.tailscaleIps;
}
const QString &Peer::relay() const
{
    return mData.relay;
}
long Peer::rxBytes() const
{
    return mData.rxBytes;
}
long Peer::txBytes() const
{
    return mData.txBytes;
}
const QDateTime &Peer::created() const
{
    return mCreated;
}
const QDateTime &Peer::lastSeen() const
{
    return mLastSeen;
}
bool Peer::isOnline() const
{
    return mData.isOnline;
}
bool Peer::isActive() const
{
    return mData.isActive;
}
bool Peer::isCurrentExitNode() const
{
    return mData.isCurrentExitNode;
}
bool Peer::isExitNode() const
{
    return mData.isExitNode;
}
const QStringList &Peer::sshHostKeys() const
{
    return mData.sshHostKeys;
}
bool Peer::isRunningSSH() const
{
    return mData.isRunningSSH();
}
QString Peer::sshCommand() const
{
    return mData.sshCommand();
}
const QStringList &Peer::tags() const
{
    return mData.tags;
}
bool Peer::isMullvad() const
{
    return mData.isMullvad;
}
Location *Peer::location() const
{
    return mLocation;
}
const PeerData &Peer::peerData() const
{
    return mData;
}

void Peer::update(PeerData &newData)
{
    if (newData.id != mData.id) {
        mData.id.swap(newData.id);
        emit idChanged(mData.id);
    }
    if (newData.publicKey != mData.publicKey) {
        mData.publicKey.swap(newData.publicKey);
        emit publicKeyChanged(mData.publicKey);
    }
    if (newData.hostName != mData.hostName) {
        mData.hostName.swap(newData.hostName);
        emit hostNameChanged(mData.hostName);
    }
    if (newData.dnsName != mData.dnsName) {
        mData.dnsName.swap(newData.dnsName);
        emit dnsNameChanged(mData.dnsName);
    }
    if (newData.os != mData.os) {
        mData.os.swap(newData.os);
        emit osChanged(mData.os);
    }
    if (newData.tailscaleIps != mData.tailscaleIps) {
        mData.tailscaleIps.swap(newData.tailscaleIps);
        emit tailscaleIpsChanged(mData.tailscaleIps);
        emit sshCommandChanged(sshCommand());
    }
    if (newData.relay != mData.relay) {
        mData.relay.swap(newData.relay);
        emit relayChanged(mData.relay);
    }
    if (newData.rxBytes != mData.rxBytes) {
        mData.rxBytes = newData.rxBytes;
        emit rxBytesChanged(mData.rxBytes);
    }
    if (newData.txBytes != mData.txBytes) {
        mData.txBytes = newData.txBytes;
        emit txBytesChanged(mData.txBytes);
    }
    if (newData.created != mData.created) {
        mData.created.swap(newData.created);
        mCreated = get_date_from_string(mData.created);
        emit createdChanged(mCreated);
    }
    if (newData.lastSeen != mData.lastSeen) {
        mData.lastSeen.swap(newData.lastSeen);
        mLastSeen = get_date_from_string(mData.lastSeen);
        emit lastSeenChanged(mLastSeen);
    }
    if (newData.isOnline != mData.isOnline) {
        mData.isOnline = newData.isOnline;
        emit isOnlineChanged(mData.isOnline);
    }
    if (newData.isActive != mData.isActive) {
        mData.isActive = newData.isActive;
        emit isActiveChanged(mData.isActive);
    }
    if (newData.isCurrentExitNode != mData.isCurrentExitNode) {
        mData.isCurrentExitNode = newData.isCurrentExitNode;
        emit isCurrentExitNodeChanged(mData.isCurrentExitNode);
    }
    if (newData.isExitNode != mData.isExitNode) {
        mData.isExitNode = newData.isExitNode;
        emit isExitNodeChanged(mData.isExitNode);
    }
    if (newData.sshHostKeys != mData.sshHostKeys) {
        if (newData.sshHostKeys.empty() ^ mData.sshHostKeys.empty()) {
            emit isRunningSSHChanged(!newData.sshHostKeys.empty());
            emit sshCommandChanged(sshCommand());
        }
        mData.sshHostKeys.swap(newData.sshHostKeys);
        emit sshHostKeysChanged(mData.sshHostKeys);
    }
    if (newData.tags != mData.tags) {
        mData.tags.swap(newData.tags);
        emit tagsChanged(mData.tags);
    }
    if (newData.isMullvad != mData.isMullvad) {
        mData.isMullvad = newData.isMullvad;
        emit isMullvadChanged(mData.isMullvad);
    }
    if (mLocation == nullptr) {
        if (newData.location.has_value()) {
            mLocation = new Location(this);
            mLocation->update(newData.location.value());
            emit locationChanged(mLocation);
        }
    } else {
        if (newData.location.has_value()) {
            mLocation->update(newData.location.value());
        } else {
            mLocation->deleteLater();
            mLocation = nullptr;
            emit locationChanged(nullptr);
        }
    }
}
