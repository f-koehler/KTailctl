// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_PEER_MODEL_H
#define KTAILCTL_PEER_MODEL_H

#include "peer.h"

#include <QAbstractListModel>

class PeerModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<Peer *> mPeers;

public slots:
    void updatePeers(const QVector<Peer *> &peers);

public:
    enum Roles : int {
        TailscaleID = Qt::UserRole + 1,
        PublicKeyRole,
        HostNameRole,
        DnsNameRole,
        OsRole,
        TailscaleIpsRole,
        IsOnlineRole,
        IsActiveRole,
        IsExitNodeRole,
        IsCurrentExitNodeRole,
        SSHHostKeysRole,
        IsRunningSSHRole,
        SSHCommandRole,
        TagsRole,
        IsMullvadRole,
    };

    explicit PeerModel(QObject *parent = nullptr);
    virtual ~PeerModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif /* KTAILCTL_PEER_MODEL_H */
