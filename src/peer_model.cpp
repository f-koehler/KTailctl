// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer_model.h"

ModelPeers::ModelPeers(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ModelPeers::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_peers.size();
}
QHash<int, QByteArray> ModelPeers::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IDRole] = "id";
    roles[PublicKeyRole] = "public_key";
    roles[HostNameRole] = "host_name";
    roles[DNSNameRole] = "dns_name";
    roles[OSRole] = "os";
    roles[OnlineRole] = "online";
    roles[ActiveRole] = "active";
    return roles;
}

QVariant ModelPeers::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QStringLiteral("Invalid Index");
    }

    switch (role) {
    case IDRole:
        return m_peers[index.row()]->getID();
    case PublicKeyRole:
        return m_peers[index.row()]->getPublicKey();
    case HostNameRole:
        return m_peers[index.row()]->getHostName();
    case DNSNameRole:
        return m_peers[index.row()]->getDNSName();
    case OSRole:
        return m_peers[index.row()]->getOS();
    case OnlineRole:
        return m_peers[index.row()]->isOnline();
    case ActiveRole:
        return m_peers[index.row()]->isActive();
    default:
        return {};
    }
}

void ModelPeers::updatePeers(const QVector<Peer *> &peers)
{
    beginResetModel();
    m_peers.clear();
    for (const auto &peer : peers) {
        m_peers.append(peer);
    }
    endResetModel();
}