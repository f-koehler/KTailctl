// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_PEER_MODEL_H
#define TAILCTL_PEER_MODEL_H

#include "peer.h"

#include <QAbstractListModel>

class PeerModel : public QAbstractItemModel
{
    Q_OBJECT

private:
    QList<Peer *> m_peers;

public slots:
    void setPeers(const QList<Peer *> &peers);

public:
    enum Roles : int {
        IdRole = Qt::UserRole + 1,
        PublicKeyRole,
        HostNameRole,
        DnsNameRole,
        OsRole,
        TailscaleIPsRole,
        IsOnlineRole,
        IsActiveRole,
    };

    PeerModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // TAILCTL_PEER_MODEL_H
