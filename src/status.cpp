// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "status.h"
#include "util.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QProcess>
#include <QSet>
#include <algorithm>

Status::Status(QObject *parent)
    : QObject(parent)
    , mSelf(new Peer())
{
}

void Status::refresh(const QString &executable)
{
    QProcess process;
    const auto [command, args] = composeTailscaleCommand(executable, {"status", "--json"});
    process.start(command, args);

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
        mVersion = json["Version"].toString();
    } else {
        qWarning() << "Cannot find string \"Version\"";
    }

    if (json.contains("TUN") && json["TUN"].isBool()) {
        if (const auto is_tun = json["TUN"].toBool(); is_tun != mIsTun) {
            mIsTun = json["TUN"].toBool();
            emit isTUNChanged(mIsTun);
        }
    } else {
        qWarning() << "Cannot find bool \"TUN\"";
    }

    if (json.contains("BackendState") && json["BackendState"].isString()) {
        if (const auto backend_state = json["BackendState"].toString(); backend_state != mBackendState) {
            mBackendState = json["BackendState"].toString();
            emit backendStateChanged(mBackendState);
        }
    } else {
        qWarning() << "Cannot find string \"BackendState\"";
    }

    if (json.contains("Self") && json["Self"].isObject()) {
        if (mSelf == nullptr) {
            mSelf = new Peer();
        }
        mSelf->setTo(Peer::fromJSON(json["Self"].toObject()));
    } else {
        qWarning() << "Cannot find object \"Self\"";
    }

    mPeers.clear();
    if (json.contains("Peer") && json["Peer"].isObject()) {
        QList<Peer *> new_peers;
        QSet<QString> ids_to_keep;
        const auto peers_object = json["Peer"].toObject();
        for (const auto &key : peers_object.keys()) {
            Peer *peer = Peer::fromJSON(peers_object[key].toObject());
            auto iter = std::find_if(mPeers.begin(), mPeers.end(), [peer](const Peer *p) {
                return p->id() == peer->id();
            });
            if (iter != mPeers.end()) {
                (*iter)->setTo(peer);
                ids_to_keep.insert(peer->id());
            } else {
                new_peers.append(peer);
            }
        }
        auto iter = mPeers.begin();
        while (iter != mPeers.end()) {
            if (!ids_to_keep.contains((*iter)->id())) {
                iter = mPeers.erase(iter);
            } else {
                ++iter;
            }
        }
        for (auto peer : new_peers) {
            mPeers.append(peer);
        }
    } else {
        qWarning() << "Cannot find object \"Peer\"";
    }

    std::sort(mPeers.begin(), mPeers.end(), [](const Peer *a, const Peer *b) {
        return a->id() < b->id();
    });
    emit peersChanged(mPeers);
}

const QString &Status::version() const
{
    return mVersion;
}

bool Status::isTUN() const
{
    return mIsTun;
}

const QString &Status::backendState() const
{
    return mBackendState;
}

const Peer *Status::self() const
{
    return mSelf;
}

const QVector<Peer *> &Status::peers() const
{
    return mPeers;
}
