// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer_model.h"

void PeerModel::updatePeers(const QVector<Peer *> &peers)
{
    if (peers.size() < mPeers.size()) {
        beginRemoveRows(QModelIndex(), peers.size(), mPeers.size() - 1);
        mPeers.erase(mPeers.begin() + peers.size(), mPeers.end());
        endRemoveRows();
    }
    for (int i = 0; i < mPeers.size(); ++i) {
        mPeers[i]->setTo(peers[i]);
        emit dataChanged(index(i), index(i));
    }
    if (mPeers.size() < peers.size()) {
        beginInsertRows(QModelIndex(), mPeers.size(), peers.size() - 1);
        for (int i = mPeers.size(); i < peers.size(); ++i) {
            mPeers.push_back(peers[i]);
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

    return mPeers.size();
}

QHash<int, QByteArray> PeerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TailscaleID] = "tailscaleID";
    roles[PublicKeyRole] = "publicKey";
    roles[HostNameRole] = "hostName";
    roles[DnsNameRole] = "dnsName";
    roles[OsRole] = "os";
    roles[TailscaleIpsRole] = "tailscaleIps";
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
        return mPeers.at(index.row())->id();
    case PublicKeyRole:
        return mPeers.at(index.row())->publicKey();
    case HostNameRole:
        return mPeers.at(index.row())->hostName();
    case DnsNameRole:
        return mPeers.at(index.row())->dnsName();
    case OsRole:
        return mPeers.at(index.row())->os();
    case TailscaleIpsRole:
        return mPeers.at(index.row())->tailscaleIps();
    case IsOnlineRole:
        return mPeers.at(index.row())->isOnline();
    case IsActiveRole:
        return mPeers.at(index.row())->isActive();
    default:
        return QStringLiteral("Unknown role");
    }
}
