// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer.h"
#include "status.h"

#include <QJsonArray>

#include <algorithm>
#include <cmath>

void Peer::updateFromStatus(const Status &status)
{
    if (status.self() != nullptr) {
        if (status.self()->id() == m_id) {
            this->setTo(*status.self());
        }
        return;
    }

    auto pos = std::find_if(status.peers().begin(), status.peers().end(), [this](const Peer *peer) {
        return peer->id() == m_id;
    });
    if (pos != status.peers().end()) {
        this->setTo(**pos);
    }
}

Peer::Peer(QObject *parent)
    : QObject(parent)
{
}

Peer *Peer::fromJSON(const QJsonObject &json)
{
    Peer *peer = new Peer();
    peer->read(json);
    return peer;
}

void Peer::read(const QJsonObject &json)
{
    if (json.contains("ID") && json["ID"].isString()) {
        if (auto id = json["ID"].toString(); id != m_id) {
            m_id = id;
            emit idChanged(m_id);
        }
    } else {
        qWarning() << "Cannot find string \"ID\"";
    }

    if (json.contains("PublicKey") && json["PublicKey"].isString()) {
        if (auto public_key = json["PublicKey"].toString(); public_key != m_public_key) {
            m_public_key = public_key;
            emit publicKeyChanged(m_public_key);
        }
    } else {
        qWarning() << "Cannot find string \"PublicKey\"";
    }

    if (json.contains("HostName") && json["HostName"].isString()) {
        if (auto host_name = json["HostName"].toString(); host_name != m_host_name) {
            m_host_name = host_name;
            emit hostNameChanged(m_host_name);
        }
    } else {
        qWarning() << "Cannot find string \"HostName\"";
    }

    if (json.contains("DNSName") && json["DNSName"].isString()) {
        if (auto dns_name = json["DNSName"].toString(); dns_name != m_dns_name) {
            m_dns_name = json["DNSName"].toString();
            emit dnsNameChanged(m_dns_name);
        }
    } else {
        qWarning() << "Cannot find string \"DNSName\"";
    }

    if (json.contains("OS") && json["OS"].isString()) {
        if (auto os_name = json["OS"].toString(); os_name != m_os) {
            m_os = json["OS"].toString();
            emit osChanged(m_os);
        }
    } else {
        qWarning() << "Cannot find string \"OS\"";
    }

    if (json.contains("TailscaleIPs") && json["TailscaleIPs"].isArray()) {
        m_tailscale_ips.clear();
        for (const auto &ip : json["TailscaleIPs"].toArray()) {
            if (ip.isString()) {
                m_tailscale_ips.append(ip.toString());
            } else {
                qWarning() << "TailscaleIPs contains non-string";
            }
        }
    } else {
        qWarning() << "Cannot find array \"TailscaleIPs\"";
    }

    if (json.contains("Relay") && json["Relay"].isString()) {
        if (auto relay = json["Relay"].toString(); relay != m_relay) {
            m_relay = relay;
            emit relayChanged(m_relay);
        }
    } else {
        qWarning() << "Cannot find string \"Relay\"";
    }

    if (json.contains("RxBytes") && json["RxBytes"].isDouble()) {
        if (const auto rx_bytes = (long)std::round(json["RxBytes"].toDouble()); rx_bytes != m_rx_bytes) {
            m_rx_bytes = rx_bytes;
            emit rxBytesChanged(m_rx_bytes);
        }
    } else {
        qWarning() << "Cannot find int \"RxBytes\"";
    }

    if (json.contains("TxBytes") && json["TxBytes"].isDouble()) {
        if (const auto tx_bytes = (long)std::round(json["TxBytes"].toDouble()); tx_bytes != m_tx_bytes) {
            m_tx_bytes = tx_bytes;
            emit txBytesChanged(m_tx_bytes);
        }
    } else {
        qWarning() << "Cannot find int \"TxBytes\"";
    }

    if (json.contains("Created") && json["Created"].isString()) {
        auto created = QDateTime::fromString(json["Created"].toString(), Qt::ISODateWithMs);
        if (created != m_created) {
            m_created = created;
            emit createdChanged(m_created);
        }
    } else {
        qWarning() << "Cannot find string \"Created\"";
    }

    if (json.contains("LastSeen") && json["LastSeen"].isString()) {
        auto last_seen = QDateTime::fromString(json["LastSeen"].toString(), Qt::ISODateWithMs);
        if (last_seen != m_last_seen) {
            m_last_seen = last_seen;
            emit lastSeenChanged(m_last_seen);
        }
    } else {
        qWarning() << "Cannot find string \"LastSeen\"";
    }

    if (json.contains("Online") && json["Online"].isBool()) {
        if (const auto is_online = json["Online"].toBool(); is_online != m_is_online) {
            m_is_online = json["Online"].toBool();
            emit isOnlineChanged(m_is_online);
        }
    } else {
        qWarning() << "Cannot find bool \"Online\"";
    }

    if (json.contains("Active") && json["Active"].isBool()) {
        if (const auto is_active = json["Active"].toBool(); is_active != m_is_active) {
            m_is_active = json["Active"].toBool();
            emit isActiveChanged(m_is_active);
        }
    } else {
        qWarning() << "Cannot find bool \"Active\"";
    }
}

bool Peer::setTo(const Peer &other)
{
    bool result = false;
    if (m_id != other.m_id) {
        m_id = other.m_id;
        emit idChanged(m_id);
        result = true;
    }

    if (m_public_key != other.m_public_key) {
        m_public_key = other.m_public_key;
        emit publicKeyChanged(m_public_key);
        result = true;
    }

    if (m_host_name != other.m_host_name) {
        m_host_name = other.m_host_name;
        emit hostNameChanged(m_host_name);
        result = true;
    }

    if (m_dns_name != other.m_dns_name) {
        m_dns_name = other.m_dns_name;
        emit dnsNameChanged(m_dns_name);
        result = true;
    }

    if (m_os != other.m_os) {
        m_os = other.m_os;
        emit osChanged(m_os);
        result = true;
    }

    if (m_tailscale_ips != other.m_tailscale_ips) {
        m_tailscale_ips = other.m_tailscale_ips;
        emit tailscaleIPsChanged(m_tailscale_ips);
        result = true;
    }

    if (m_relay != other.m_relay) {
        m_relay = other.m_relay;
        emit relayChanged(m_relay);
        result = true;
    }

    if (m_rx_bytes != other.m_rx_bytes) {
        m_rx_bytes = other.m_rx_bytes;
        emit rxBytesChanged(m_rx_bytes);
        result = true;
    }

    if (m_tx_bytes != other.m_tx_bytes) {
        m_tx_bytes = other.m_tx_bytes;
        emit txBytesChanged(m_tx_bytes);
        result = true;
    }

    if (m_created != other.m_created) {
        m_created = other.m_created;
        emit createdChanged(m_created);
        result = true;
    }

    if (m_last_seen != other.m_last_seen) {
        m_last_seen = other.m_last_seen;
        emit lastSeenChanged(m_last_seen);
        result = true;
    }

    if (m_is_online != other.m_is_online) {
        m_is_online = other.m_is_online;
        emit isOnlineChanged(m_is_online);
        result = true;
    }

    if (m_is_active != other.m_is_active) {
        m_is_active = other.m_is_active;
        emit isActiveChanged(m_is_active);
        result = true;
    }

    return result;
}

const QString &Peer::id() const
{
    return m_id;
}

const QString &Peer::publicKey() const
{
    return m_public_key;
}

const QString &Peer::hostName() const
{
    return m_host_name;
}

const QString &Peer::dnsName() const
{
    return m_dns_name;
}

const QString &Peer::os() const
{
    return m_os;
}

const QStringList &Peer::tailscaleIPs() const
{
    return m_tailscale_ips;
}

const QString &Peer::relay() const
{
    return m_relay;
}

long Peer::rxBytes() const
{
    return m_rx_bytes;
}

long Peer::txBytes() const
{
    return m_tx_bytes;
}

const QDateTime &Peer::created() const
{
    return m_created;
}

const QDateTime &Peer::lastSeen() const
{
    return m_last_seen;
}

bool Peer::isOnline() const
{
    return m_is_online;
}

bool Peer::isActive() const
{
    return m_is_active;
}

Peer &Peer::operator=(const Peer &other)
{
    this->setTo(other);
    return *this;
}