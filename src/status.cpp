// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "status.h"

#include <QProcess>
#include <QJsonArray>
#include <QJsonDocument>

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
