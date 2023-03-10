// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_STATUS_H
#define TAILCTL_STATUS_H

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QList>
#include <QObject>

#include "peer.h"

struct Status : public QObject {
    Q_OBJECT

private:
  QString m_version;
  bool m_tun;
  QString m_backend_state;
  Peer m_self;
  QVector<Peer> m_peers;

protected:
    void read(const QJsonObject& json);

public slots:
    void refresh(const QString& executable = QStringLiteral("tailscale"));

public:
    const QString& getVersion() const;
    bool isTUN() const;
    const QString& getBackendState() const;
    const Peer& getSelf() const;
    const QVector<Peer> & getPeers() const;
};

#endif /* TAILCTL_STATUS_H */
