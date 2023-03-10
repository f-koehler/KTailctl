// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "status.h"

#include <QProcess>
#include <QtDebug>
#include <QJsonArray>
#include <QJsonDocument>

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



void Status::refresh(const QString& executable) {
    QProcess process;
    process.start(executable, {"status", "--json"});

    if(!process.waitForFinished(1000)) {
        qWarning() << "Failed to get tailscale status";
        return;
    }

    read(QJsonDocument::fromJson(process.readAllStandardOutput()).object());
}

void Status::read(const QJsonObject &json) {
    if(json.contains("Version") && json["Version"].isString()) {
        m_version = json["Version"].toString();
    } else {
        qWarning() << "Cannot find string \"Version\"";
    }

    if(json.contains("TUN") && json["TUN"].isBool()) {
        m_tun = json["TUN"].toBool();
    } else {
        qWarning() << "Cannot find bool \"TUN\"";
    }

    if(json.contains("BackendState") && json["BackendState"].isString()) {
        m_backend_state = json["BackendState"].toString();
    } else {
        qWarning() << "Cannot find string \"BackendState\"";
    }

    if(json.contains("Self") && json["Self"].isObject()) {
        m_self.read(json["Self"].toObject());
    } else {
        qWarning() << "Cannot find object \"Self\"";
    }

    if(json.contains("Peer") && json["Peer"].isObject()) {
        auto peers_obj = json["Peer"].toObject();
        m_peers.resize(peers_obj.size());
        int i = 0;
        for(const auto& peer_key : json["Peer"].toObject().keys()) {
            if(peers_obj[peer_key].isObject()) {
                m_peers[i].read(peers_obj[peer_key].toObject());
            } else {
                qWarning() << "Peer list contains non-object";
            }
        }
    } else {
        qWarning() << "Cannot find object \"Peer\"";
    }
}

const QString &Status::getVersion() const {
    return m_version;
}

bool Status::isTUN() const {
    return m_tun;
}

const QString &Status::getBackendState() const {
    return m_backend_state;
}

const Peer &Status::getSelf() const {
    return m_self;
}

const QVector<Peer> & Status::getPeers() const {
    return m_peers;
}
