// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer.h"

#include <QJsonArray>

void Peer::read(const QJsonObject &json) {
    if(json.contains("ID") && json["ID"].isString()) {
        m_id = json["ID"].toString();
    } else {
        qWarning() << "Cannot find string \"ID\"";
    }

    if(json.contains("PublicKey") && json["PublicKey"].isString()) {
        m_public_key = json["PublicKey"].toString();
    } else {
        qWarning() << "Cannot find string \"PublicKey\"";
    }

    if(json.contains("HostName") && json["HostName"].isString()) {
        m_host_name = json["HostName"].toString();
    } else {
        qWarning() << "Cannot find string \"HostName\"";
    }

    if(json.contains("DNSName") && json["DNSName"].isString()) {
        m_dns_name = json["DNSName"].toString();
    } else {
        qWarning() << "Cannot find string \"DNSName\"";
    }

    if(json.contains("OS") && json["OS"].isString()) {
        m_os = json["OS"].toString();
    } else {
        qWarning() << "Cannot find string \"OS\"";
    }

    if(json.contains("TailscaleIPs") && json["TailscaleIPs"].isArray()) {
        m_tailscale_ips.clear();
        for(const auto& ip : json["TailscaleIPs"].toArray()) {
            if(ip.isString()) {
                m_tailscale_ips.append(ip.toString());
            } else {
                qWarning() << "TailscaleIPs contains non-string";
            }
        }
    } else {
        qWarning() << "Cannot find array \"TailscaleIPs\"";
    }

    if(json.contains("Online") && json["Online"].isBool()) {
        m_online = json["Online"].toBool();
    } else {
        qWarning() << "Cannot find bool \"Online\"";
    }

    if(json.contains("Active") && json["Active"].isBool()) {
        m_active = json["Active"].toBool();
    } else {
        qWarning() << "Cannot find bool \"Active\"";
    }
}

const QString &Peer::getID() const {
    return m_id;
}

const QString &Peer::getPublicKey() const {
    return m_public_key;
}

const QString &Peer::getHostName() const {
    return m_host_name;
}

const QString &Peer::getDNSName() const {
    return m_dns_name;
}

const QString &Peer::getOS() const {
    return m_os;
}

const QVector<QString> &Peer::getTailscaleIPs() const {
    return m_tailscale_ips;
}

bool Peer::isOnline() const {
    return m_online;
}

bool Peer::isActive() const {
    return m_active;
}
