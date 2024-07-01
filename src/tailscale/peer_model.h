// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_MODELS_PEER_MODEL_H
#define KTAILCTL_MODELS_PEER_MODEL_H

#include "peer.h"
#include "peer_data.h"

#include <QAbstractListModel>
#include <QVector>

class Tailscale;

class PeerModel : public QAbstractListModel
{
    Q_OBJECT

    friend class Tailscale;

    // private:
    //     QVector<PeerData> mData;

    // public slots:
    //     void updatePeers(const Status &status);
    //
private:
    const QVector<Peer *> *mPeerList;

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

    explicit PeerModel(const QVector<Peer *> *peerList, QObject *parent = nullptr);
    virtual ~PeerModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif /* KTAILCTL_MODELS_PEER_MODEL_H */
