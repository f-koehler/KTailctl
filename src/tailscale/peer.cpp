#include "peer.h"

static constexpr const char *TAILSCALE_JSON_NOWDATE = "0001-01-01T00:00:00Z";
QDateTime get_date_from_string(const QString &string)
{
    if (string == TAILSCALE_JSON_NOWDATE) {
        return QDateTime::currentDateTime();
    }
    return QDateTime::fromString(string, Qt::ISODateWithMs);
}

Peer::Peer(QObject *parent)
    : QObject(parent)
    , mData()
    , mLocation(new Location(this))
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

bool Peer::update(PeerData &newData)
{
    bool changed = false;
    if (newData.id != mData.id) {
        changed = true;
        mData.id.swap(newData.id);
        emit idChanged(mData.id);
    }
    if (newData.publicKey != mData.publicKey) {
        changed = true;
        mData.publicKey.swap(newData.publicKey);
        emit publicKeyChanged(mData.publicKey);
    }
    if (newData.hostName != mData.hostName) {
        changed = true;
        mData.hostName.swap(newData.hostName);
        emit hostNameChanged(mData.hostName);
    }
    if (newData.dnsName != mData.dnsName) {
        changed = true;
        mData.dnsName.swap(newData.dnsName);
        emit dnsNameChanged(mData.dnsName);
    }
    if (newData.os != mData.os) {
        changed = true;
        mData.os.swap(newData.os);
        emit osChanged(mData.os);
    }
    if (newData.tailscaleIps != mData.tailscaleIps) {
        changed = true;
        mData.tailscaleIps.swap(newData.tailscaleIps);
        emit tailscaleIpsChanged(mData.tailscaleIps);
        emit sshCommandChanged(sshCommand());
    }
    if (newData.relay != mData.relay) {
        changed = true;
        mData.relay.swap(newData.relay);
        emit relayChanged(mData.relay);
    }
    if (newData.rxBytes != mData.rxBytes) {
        changed = true;
        mData.rxBytes = newData.rxBytes;
        emit rxBytesChanged(mData.rxBytes);
    }
    if (newData.txBytes != mData.txBytes) {
        changed = true;
        mData.txBytes = newData.txBytes;
        emit txBytesChanged(mData.txBytes);
    }
    if (newData.created != mData.created) {
        changed = true;
        mData.created.swap(newData.created);
        mCreated = get_date_from_string(mData.created);
        emit createdChanged(mCreated);
    }
    if (newData.lastSeen != mData.lastSeen) {
        changed = true;
        mData.lastSeen.swap(newData.lastSeen);
        mLastSeen = get_date_from_string(mData.lastSeen);
        emit lastSeenChanged(mLastSeen);
    }
    if (newData.isOnline != mData.isOnline) {
        changed = true;
        mData.isOnline = newData.isOnline;
        emit isOnlineChanged(mData.isOnline);
    }
    if (newData.isActive != mData.isActive) {
        changed = true;
        mData.isActive = newData.isActive;
        emit isActiveChanged(mData.isActive);
    }
    if (newData.isCurrentExitNode != mData.isCurrentExitNode) {
        changed = true;
        mData.isCurrentExitNode = newData.isCurrentExitNode;
        emit isCurrentExitNodeChanged(mData.isCurrentExitNode);
    }
    if (newData.isExitNode != mData.isExitNode) {
        changed = true;
        mData.isExitNode = newData.isExitNode;
        emit isExitNodeChanged(mData.isExitNode);
    }
    if (newData.sshHostKeys != mData.sshHostKeys) {
        changed = true;
        if (newData.sshHostKeys.empty() ^ mData.sshHostKeys.empty()) {
            emit isRunningSSHChanged(!newData.sshHostKeys.empty());
            emit sshCommandChanged(sshCommand());
        }
        mData.sshHostKeys.swap(newData.sshHostKeys);
        emit sshHostKeysChanged(mData.sshHostKeys);
    }
    if (newData.tags != mData.tags) {
        changed = true;
        mData.tags.swap(newData.tags);
        emit tagsChanged(mData.tags);
    }
    if (newData.isMullvad != mData.isMullvad) {
        changed = true;
        mData.isMullvad = newData.isMullvad;
        emit isMullvadChanged(mData.isMullvad);
    }
    if (mLocation == nullptr) {
        if (newData.location.has_value()) {
            changed = true;
            mLocation = new Location(this);
            mLocation->update(newData.location.value());
            emit locationChanged(mLocation);
        }
    } else {
        if (newData.location.has_value()) {
            if (mLocation->update(newData.location.value())) {
                changed = true;
            }
        } else {
            changed = true;
            mLocation->deleteLater();
            mLocation = nullptr;
            emit locationChanged(nullptr);
        }
    }
    return changed;
}
