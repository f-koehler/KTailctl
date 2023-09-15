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

    if (json.contains("ID") && json["ID"].isString()) {
        peer->mId = json["ID"].toString();
    } else {
        qWarning() << "Cannot find string \"ID\"";
    }

    if (json.contains("PublicKey") && json["PublicKey"].isString()) {
        peer->mPublicKey = json["PublicKey"].toString();
    } else {
        qWarning() << "Cannot find string \"PublicKey\"";
    }

    if (json.contains("HostName") && json["HostName"].isString()) {
        peer->mHostName = json["HostName"].toString();
    } else {
        qWarning() << "Cannot find string \"HostName\"";
    }

    if (json.contains("DNSName") && json["DNSName"].isString()) {
        peer->mDNSName = json["DNSName"].toString();
    } else {
        qWarning() << "Cannot find string \"DNSName\"";
    }

    if (json.contains("OS") && json["OS"].isString()) {
        peer->mOs = json["OS"].toString();
    } else {
        qWarning() << "Cannot find string \"OS\"";
    }

    if (json.contains("TailscaleIPs") && json["TailscaleIPs"].isArray()) {
        peer->mTailscaleIps.clear();
        for (const auto &ip : json["TailscaleIPs"].toArray()) {
            if (ip.isString()) {
                peer->mTailscaleIps.append(ip.toString());
            } else {
                qWarning() << "TailscaleIPs contains non-string";
            }
        }
    } else {
        qWarning() << "Cannot find array \"TailscaleIPs\"";
    }

    if (json.contains("Relay") && json["Relay"].isString()) {
        peer->mRelay = json["Relay"].toString();
    } else {
        qWarning() << "Cannot find string \"Relay\"";
    }

    if (json.contains("RxBytes") && json["RxBytes"].isDouble()) {
        peer->mRxBytes = (long)std::round(json["RxBytes"].toDouble());
    } else {
        qWarning() << "Cannot find int \"RxBytes\"";
    }

    if (json.contains("TxBytes") && json["TxBytes"].isDouble()) {
        peer->mTxBytes = (long)std::round(json["TxBytes"].toDouble());
    } else {
        qWarning() << "Cannot find int \"TxBytes\"";
    }

    static constexpr const char *nowdate = "0001-01-01T00:00:00Z";
    if (json.contains("Created") && json["Created"].isString()) {
        const auto string = json["Created"].toString();
        if (string == nowdate) {
            peer->mCreated = QDateTime::currentDateTime();
        } else {
            peer->mCreated = QDateTime::fromString(string, Qt::ISODateWithMs);
        }
    } else {
        qWarning() << "Cannot find string \"Created\"";
    }

    if (json.contains("LastSeen") && json["LastSeen"].isString()) {
        const auto string = json["LastSeen"].toString();
        if (string == nowdate) {
            peer->mLastSeen = QDateTime::currentDateTime();
        } else {
            peer->mLastSeen = QDateTime::fromString(string, Qt::ISODateWithMs);
        }
    } else {
        qWarning() << "Cannot find string \"LastSeen\"";
    }

    if (json.contains("Online") && json["Online"].isBool()) {
        peer->mIsOnline = json["Online"].toBool();
    } else {
        qWarning() << "Cannot find bool \"Online\"";
    }

    if (json.contains("Active") && json["Active"].isBool()) {
        peer->mIsActive = json["Active"].toBool();
    } else {
        qWarning() << "Cannot find bool \"Active\"";
    }

    return peer;
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
    this->setTo(&other);
    return *this;
}
