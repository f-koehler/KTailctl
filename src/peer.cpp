// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer.h"
#include "status.h"

#include <QJsonArray>

#include <algorithm>
#include <cmath>

void Peer::updateFromStatus(const Status &status)
{
    if (status.self() != nullptr) {
        if (status.self()->id() == mId) {
            this->setTo(status.self());
            return;
        }
    }

    const auto *pos = std::find_if(status.peers().begin(), status.peers().end(), [this](const Peer *peer) {
        return peer->id() == mId;
    });
    if (pos != status.peers().end()) {
        this->setTo(*pos);
    }
}

Peer::Peer(QObject *parent)
    : QObject(parent)
{
}

Peer *Peer::fromJSON(const QJsonObject &json)
{
    Peer *peer = new Peer();
    peer->setIdFromJSON(json);
    peer->setPublicKeyFromJSON(json);
    peer->setHostNameFromJSON(json);
    peer->setDNSNameFromJSON(json);
    peer->setOsFromJSON(json);
    peer->setTailscaleIpsFromJSON(json);
    peer->setRelayFromJSON(json);
    peer->setRxBytesFromJSON(json);
    peer->setTxBytesFromJSON(json);
    peer->setCreatedFromJSON(json);
    peer->setLastSeenFromJSON(json);
    peer->setIsOnlineFromJSON(json);
    peer->setIsActiveFromJSON(json);

    return peer;
}

void Peer::setIdFromJSON(const QJsonObject &json)
{
    if (json.contains("ID") && json["ID"].isString()) {
        mId = json["ID"].toString();
    } else {
        qWarning() << "Cannot find string \"ID\"";
    }
}
void Peer::setPublicKeyFromJSON(const QJsonObject &json)
{
    if (json.contains("PublicKey") && json["PublicKey"].isString()) {
        mPublicKey = json["PublicKey"].toString();
    } else {
        qWarning() << "Cannot find string \"PublicKey\"";
    }
}
void Peer::setHostNameFromJSON(const QJsonObject &json)
{
    if (json.contains("HostName") && json["HostName"].isString()) {
        mHostName = json["HostName"].toString();
    } else {
        qWarning() << "Cannot find string \"HostName\"";
    }
}
void Peer::setDNSNameFromJSON(const QJsonObject &json)
{
    if (json.contains("DNSName") && json["DNSName"].isString()) {
        mDNSName = json["DNSName"].toString();
    } else {
        qWarning() << "Cannot find string \"DNSName\"";
    }
}
void Peer::setOsFromJSON(const QJsonObject &json)
{
    if (json.contains("OS") && json["OS"].isString()) {
        mOs = json["OS"].toString();
    } else {
        qWarning() << "Cannot find string \"OS\"";
    }
}
void Peer::setTailscaleIpsFromJSON(const QJsonObject &json)
{
    if (json.contains("TailscaleIPs") && json["TailscaleIPs"].isArray()) {
        mTailscaleIps.clear();
        for (const auto &ip_address : json["TailscaleIPs"].toArray()) {
            if (ip_address.isString()) {
                mTailscaleIps.append(ip_address.toString());
            } else {
                qWarning() << "TailscaleIPs contains non-string";
            }
        }
    } else {
        qWarning() << "Cannot find array \"TailscaleIPs\"";
    }
}
void Peer::setRelayFromJSON(const QJsonObject &json)
{
    if (json.contains("Relay") && json["Relay"].isString()) {
        mRelay = json["Relay"].toString();
    } else {
        qWarning() << "Cannot find string \"Relay\"";
    }
}
void Peer::setRxBytesFromJSON(const QJsonObject &json)
{
    if (json.contains("RxBytes") && json["RxBytes"].isDouble()) {
        mRxBytes = (long)std::round(json["RxBytes"].toDouble());
    } else {
        qWarning() << "Cannot find int \"RxBytes\"";
    }
}
void Peer::setTxBytesFromJSON(const QJsonObject &json)
{
    if (json.contains("TxBytes") && json["TxBytes"].isDouble()) {
        mTxBytes = (long)std::round(json["TxBytes"].toDouble());
    } else {
        qWarning() << "Cannot find int \"TxBytes\"";
    }
}
void Peer::setCreatedFromJSON(const QJsonObject &json)
{
    static constexpr const char *nowdate = "0001-01-01T00:00:00Z";
    if (json.contains("Created") && json["Created"].isString()) {
        const auto string = json["Created"].toString();
        if (string == nowdate) {
            mCreated = QDateTime::currentDateTime();
        } else {
            mCreated = QDateTime::fromString(string, Qt::ISODateWithMs);
        }
    } else {
        qWarning() << "Cannot find string \"Created\"";
    }
}
void Peer::setLastSeenFromJSON(const QJsonObject &json)
{
    static constexpr const char *nowdate = "0001-01-01T00:00:00Z";
    if (json.contains("LastSeen") && json["LastSeen"].isString()) {
        const auto string = json["LastSeen"].toString();
        if (string == nowdate) {
            mLastSeen = QDateTime::currentDateTime();
        } else {
            mLastSeen = QDateTime::fromString(string, Qt::ISODateWithMs);
        }
    } else {
        qWarning() << "Cannot find string \"LastSeen\"";
    }
}
void Peer::setIsOnlineFromJSON(const QJsonObject &json)
{
    if (json.contains("Online") && json["Online"].isBool()) {
        mIsOnline = json["Online"].toBool();
    } else {
        qWarning() << "Cannot find bool \"Online\"";
    }
}
void Peer::setIsActiveFromJSON(const QJsonObject &json)
{
    if (json.contains("Active") && json["Active"].isBool()) {
        mIsActive = json["Active"].toBool();
    } else {
        qWarning() << "Cannot find bool \"Active\"";
    }
}

bool Peer::setTo(const Peer *other)
{
    bool result = false;
    if (mId != other->mId) {
        mId = other->mId;
        emit idChanged(mId);
        result = true;
    }

    if (mPublicKey != other->mPublicKey) {
        mPublicKey = other->mPublicKey;
        emit publicKeyChanged(mPublicKey);
        result = true;
    }

    if (mHostName != other->mHostName) {
        mHostName = other->mHostName;
        emit hostNameChanged(mHostName);
        result = true;
    }

    if (mDNSName != other->mDNSName) {
        mDNSName = other->mDNSName;
        emit dnsNameChanged(mDNSName);
        result = true;
    }

    if (mOs != other->mOs) {
        mOs = other->mOs;
        emit osChanged(mOs);
        result = true;
    }

    if (mTailscaleIps != other->mTailscaleIps) {
        mTailscaleIps = other->mTailscaleIps;
        emit tailscaleIpsChanged(mTailscaleIps);
        result = true;
    }

    if (mRelay != other->mRelay) {
        mRelay = other->mRelay;
        emit relayChanged(mRelay);
        result = true;
    }

    if (mRxBytes != other->mRxBytes) {
        mRxBytes = other->mRxBytes;
        emit rxBytesChanged(mRxBytes);
        result = true;
    }

    if (mTxBytes != other->mTxBytes) {
        mTxBytes = other->mTxBytes;
        emit txBytesChanged(mTxBytes);
        result = true;
    }

    if (mCreated != other->mCreated) {
        mCreated = other->mCreated;
        emit createdChanged(mCreated);
        result = true;
    }

    if (mLastSeen != other->mLastSeen) {
        mLastSeen = other->mLastSeen;
        emit lastSeenChanged(mLastSeen);
        result = true;
    }

    if (mIsOnline != other->mIsOnline) {
        mIsOnline = other->mIsOnline;
        emit isOnlineChanged(mIsOnline);
        result = true;
    }

    if (mIsActive != other->mIsActive) {
        mIsActive = other->mIsActive;
        emit isActiveChanged(mIsActive);
        result = true;
    }

    return result;
}

const QString &Peer::id() const
{
    return mId;
}

const QString &Peer::publicKey() const
{
    return mPublicKey;
}

const QString &Peer::hostName() const
{
    return mHostName;
}

const QString &Peer::dnsName() const
{
    return mDNSName;
}

const QString &Peer::os() const
{
    return mOs;
}

const QStringList &Peer::tailscaleIps() const
{
    return mTailscaleIps;
}

const QString &Peer::relay() const
{
    return mRelay;
}

long Peer::rxBytes() const
{
    return mRxBytes;
}

long Peer::txBytes() const
{
    return mTxBytes;
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
    return mIsOnline;
}

bool Peer::isActive() const
{
    return mIsActive;
}

Peer &Peer::operator=(const Peer &other)
{
    if (this == &other) {
        return *this;
    }
    this->setTo(&other);
    return *this;
}
