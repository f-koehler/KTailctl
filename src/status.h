// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_STATUS_H
#define TAILCTL_STATUS_H

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QList>


#include "peer.h"

struct Status {
    Q_GADGET

private:
  QString m_version;
  bool m_tun;
  QString m_backend_state;
  Peer m_self;
  QVector<Peer> m_peers;

public:
    void refresh(const QString& executable = QStringLiteral("tailscale"));
    void read(const QJsonObject& json);

    const QString& getVersion() const;
    bool isTUN() const;
    const QString& getBackendState() const;
    const Peer& getSelf() const;
    const QVector<Peer> & getPeers() const;
};

#endif /* TAILCTL_STATUS_H */
