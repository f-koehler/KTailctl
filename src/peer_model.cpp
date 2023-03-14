// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer_model.h"

void PeerModel::setPeers(const QList<Peer *> &peers)
{
    beginResetModel();
    m_peers = peers;
    endResetModel();
    emit dataChanged(index(0, 0), index(m_peers.size(), 0));
}

PeerModel::PeerModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

int PeerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_peers.size();
}

int PeerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

QVariant PeerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    switch (role) {
    case IdRole:
        return m_peers.at(index.row())->id();
    case PublicKeyRole:
        return m_peers.at(index.row())->publicKey();
    case HostNameRole:
        return m_peers.at(index.row())->hostName();
    case DnsNameRole:
        return m_peers.at(index.row())->dnsName();
    case OsRole:
        return m_peers.at(index.row())->os();
    case IsOnlineRole:
        return m_peers.at(index.row())->isOnline();
    case IsActiveRole:
        return m_peers.at(index.row())->isActive();
    default:
        return QVariant();
    }
}

QModelIndex PeerModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex PeerModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}
