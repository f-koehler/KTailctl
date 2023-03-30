// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer_model.h"

void PeerModel::updatePeers(const QVector<Peer *> &peers)
{
    if (peers.size() < m_peers.size()) {
        beginRemoveRows(QModelIndex(), peers.size(), m_peers.size() - 1);
        m_peers.erase(m_peers.begin() + peers.size(), m_peers.end());
        endRemoveRows();
    }
    for (int i = 0; i < m_peers.size(); ++i) {
        m_peers[i]->setTo(peers[i]);
        emit dataChanged(index(i), index(i));
    }
    if (m_peers.size() < peers.size()) {
        beginInsertRows(QModelIndex(), m_peers.size(), peers.size() - 1);
        for (int i = m_peers.size(); i < peers.size(); ++i) {
            m_peers.push_back(peers[i]);
        }
        endInsertRows();
    }
}

PeerModel::PeerModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PeerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_peers.size();
}

QHash<int, QByteArray> PeerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TailscaleID] = "tailscaleID";
    roles[PublicKeyRole] = "publicKey";
    roles[HostNameRole] = "hostName";
    roles[DnsNameRole] = "dnsName";
    roles[OsRole] = "os";
    roles[IsOnlineRole] = "isOnline";
    roles[IsActiveRole] = "isActive";
    return roles;
}

QVariant PeerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QStringLiteral("Invalid index");
    }
    switch (role) {
    case TailscaleID:
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
        return QStringLiteral("Unknown role");
    }
}
