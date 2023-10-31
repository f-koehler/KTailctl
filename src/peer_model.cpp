// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer_model.h"

void PeerModel::updatePeers(const Status &status)
{
    const auto newData = status.statusData().peers;
    if (mData.size() > newData.size()) {
        beginRemoveRows(QModelIndex(), newData.size(), mData.size() - 1);
        mData.erase(mData.begin() + newData.size(), mData.end());
        endRemoveRows();
    }
    for (int i = 0; i < mData.size(); ++i) {
        if (mData[i] == newData[i]) {
            continue;
        }
        mData[i] = newData[i];
        emit dataChanged(index(i), index(i));
    }
    if (mData.size() < newData.size()) {
        beginInsertRows(QModelIndex(), mData.size(), newData.size() - 1);
        std::copy(newData.begin() + mData.size(), newData.end(), std::back_inserter(mData));
        endInsertRows();
    }
    qWarning() << newData.size();
}

PeerModel::PeerModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PeerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mData.size();
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
    roles[IsExitNodeRole] = "isExitNode";
    roles[IsCurrentExitNodeRole] = "isCurrentExitNode";
    roles[SSHHostKeysRole] = "sshHostKeys";
    roles[IsRunningSSHRole] = "isRunningSSH";
    roles[SSHCommandRole] = "sshCommand";
    roles[TagsRole] = "tags";
    roles[IsMullvadRole] = "isMullvad";
    return roles;
}

QVariant PeerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QStringLiteral("Invalid index");
    }
    switch (role) {
    case TailscaleID:
        return mData.at(index.row()).id;
    case PublicKeyRole:
        return mData.at(index.row()).publicKey;
    case HostNameRole:
        return mData.at(index.row()).hostName;
    case DnsNameRole:
        return mData.at(index.row()).dnsName;
    case OsRole:
        return mData.at(index.row()).os;
    case TailscaleIpsRole:
        return mData.at(index.row()).tailscaleIps;
    case IsOnlineRole:
        return mData.at(index.row()).isOnline;
    case IsActiveRole:
        return mData.at(index.row()).isActive;
    case IsExitNodeRole:
        return mData.at(index.row()).isExitNode;
    case IsCurrentExitNodeRole:
        return mData.at(index.row()).isCurrentExitNode;
    case SSHHostKeysRole:
        return mData.at(index.row()).sshHostKeys;
    case IsRunningSSHRole:
        return mData.at(index.row()).isRunningSSH();
    case SSHCommandRole:
        return mData.at(index.row()).sshCommand();
    case TagsRole:
        return mData.at(index.row()).tags;
    case IsMullvadRole:
        return mData.at(index.row()).isMullvad;
    default:
        return QStringLiteral("Unknown role");
    }
}
