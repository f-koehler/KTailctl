// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_PEER_MODEL_H
#define TAILCTL_PEER_MODEL_H

#include "peer.h"

#include <QAbstractListModel>

class ModelPeers : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles { IDRole = Qt::UserRole + 1, PublicKeyRole, HostNameRole, DNSNameRole, OSRole, OnlineRole, ActiveRole };

private:
    QVector<Peer *> m_peers;

public:
    explicit ModelPeers(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void updatePeers(const QVector<Peer *> &peers);
};

#endif // TAILCTL_PEER_MODEL_H
