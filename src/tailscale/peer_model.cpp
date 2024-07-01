// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer_model.h"

PeerModel::PeerModel(const QVector<Peer *> *peerList, QObject *parent)
    : QAbstractListModel(parent)
    , mPeerList(peerList)
{
}

int PeerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mPeerList->size();
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
        return mPeerList->at(index.row())->id();
    case PublicKeyRole:
        return mPeerList->at(index.row())->publicKey();
    case HostNameRole:
        return mPeerList->at(index.row())->hostName();
    case DnsNameRole:
        return mPeerList->at(index.row())->dnsName();
    case OsRole:
        return mPeerList->at(index.row())->os();
    case TailscaleIpsRole:
        return mPeerList->at(index.row())->tailscaleIps();
    case IsOnlineRole:
        return mPeerList->at(index.row())->isOnline();
    case IsActiveRole:
        return mPeerList->at(index.row())->isActive();
    case IsExitNodeRole:
        return mPeerList->at(index.row())->isExitNode();
    case IsCurrentExitNodeRole:
        return mPeerList->at(index.row())->isCurrentExitNode();
    case SSHHostKeysRole:
        return mPeerList->at(index.row())->sshHostKeys();
    case IsRunningSSHRole:
        return mPeerList->at(index.row())->isRunningSSH();
    case SSHCommandRole:
        return mPeerList->at(index.row())->sshCommand();
    case TagsRole:
        return mPeerList->at(index.row())->tags();
    case IsMullvadRole:
        return mPeerList->at(index.row())->isMullvad();
    default:
        return QStringLiteral("Unknown role");
    }
}
