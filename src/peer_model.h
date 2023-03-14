// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_PEER_MODEL_H
#define TAILCTL_PEER_MODEL_H

#include "peer.h"

#include <QAbstractListModel>

class PeerModel : public QAbstractListModel {
  Q_OBJECT

private:
  QList<Peer *> m_peers;

public slots:
  void updatePeers(const QList<Peer *> &peers);

public:
  enum Roles : int {
    IdRole = Qt::UserRole + 1,
    PublicKeyRole,
    HostNameRole,
    DnsNameRole,
    OsRole,
    IsOnlineRole,
    IsActiveRole,
  };

  explicit PeerModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent) const override;
  QHash<int, QByteArray> roleNames() const override;
  QVariant data(const QModelIndex &index, int role) const override;
};

#endif // TAILCTL_PEER_MODEL_H
