// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "status.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QProcess>
#include <QSet>
#include <algorithm>

Status::Status(QObject *parent)
    : QObject(parent)
    , m_self(new Peer())
{
}

void Status::refresh(const QString &executable)
{
    QProcess process;
    process.start(executable, {"status", "--json"});

    if (!process.waitForFinished(1000)) {
        qWarning() << "Failed to get tailscale status";
        return;
    }

    read(QJsonDocument::fromJson(process.readAllStandardOutput()).object());

    emit refreshed(*this);
}

void Status::read(const QJsonObject &json)
{
    if (json.contains("Version") && json["Version"].isString()) {
        m_version = json["Version"].toString();
    } else {
        qWarning() << "Cannot find string \"Version\"";
    }

    if (json.contains("TUN") && json["TUN"].isBool()) {
        if (const auto is_tun = json["TUN"].toBool(); is_tun != m_is_tun) {
            m_is_tun = json["TUN"].toBool();
            emit isTUNChanged(m_is_tun);
        }
    } else {
        qWarning() << "Cannot find bool \"TUN\"";
    }

    if (json.contains("BackendState") && json["BackendState"].isString()) {
        if (const auto backend_state = json["BackendState"].toString(); backend_state != m_backend_state) {
            m_backend_state = json["BackendState"].toString();
            emit backendStateChanged(m_backend_state);
        }
    } else {
        qWarning() << "Cannot find string \"BackendState\"";
    }

    if (json.contains("Self") && json["Self"].isObject()) {
        if (m_self == nullptr) {
            m_self = new Peer();
        }
        m_self->setTo(Peer::fromJSON(json["Self"].toObject()));
    } else {
        qWarning() << "Cannot find object \"Self\"";
    }

    m_peers.clear();
    if (json.contains("Peer") && json["Peer"].isObject()) {
        QList<Peer *> new_peers;
        QSet<QString> ids_to_keep;
        const auto peers_object = json["Peer"].toObject();
        for (const auto &key : peers_object.keys()) {
            Peer *peer = Peer::fromJSON(peers_object[key].toObject());
            auto iter = std::find_if(m_peers.begin(), m_peers.end(), [peer](const Peer *p) {
                return p->id() == peer->id();
            });
            if (iter != m_peers.end()) {
                (*iter)->setTo(peer);
                ids_to_keep.insert(peer->id());
            } else {
                new_peers.append(peer);
            }
        }
        auto iter = m_peers.begin();
        while (iter != m_peers.end()) {
            if (!ids_to_keep.contains((*iter)->id())) {
                iter = m_peers.erase(iter);
            } else {
                ++iter;
            }
        }
        for (auto peer : new_peers) {
            m_peers.append(peer);
        }
    } else {
        qWarning() << "Cannot find object \"Peer\"";
    }

    std::sort(m_peers.begin(), m_peers.end(), [](const Peer *a, const Peer *b) {
        return a->id() < b->id();
    });
    emit peersChanged(m_peers);
}

const QString &Status::version() const
{
    return m_version;
}

bool Status::isTUN() const
{
    return m_is_tun;
}

const QString &Status::backendState() const
{
    return m_backend_state;
}

const Peer *Status::self() const
{
    return m_self;
}

const QVector<Peer *> &Status::peers() const
{
    return m_peers;
}
