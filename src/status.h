// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_STATUS_H
#define TAILCTL_STATUS_H

#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <QVector>

#include "peer.h"

struct Status : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString version READ version NOTIFY versionChanged)
  Q_PROPERTY(bool isTUN READ isTUN NOTIFY isTUNChanged)
  Q_PROPERTY(QString backendState READ backendState NOTIFY backendStateChanged)
  Q_PROPERTY(QList<Peer *> peers READ peers NOTIFY peersChanged)

private:
  QString m_version;
  bool m_is_tun;
  QString m_backend_state;
  Peer *m_self;
  QList<Peer *> m_peers;

signals:
  void versionChanged(const QString &);
  void isTUNChanged(bool);
  void backendStateChanged(const QString &);
  void peersChanged(const QList<Peer *> &);

public:
  Status(QObject *parent = nullptr);
  virtual ~Status() = default;

  Q_INVOKABLE void
  refresh(const QString &executable = QStringLiteral("tailscale"));
  void read(const QJsonObject &json);

  const QString &version() const;
  bool isTUN() const;
  const QString &backendState() const;
  const Peer *self() const;
  const QList<Peer *> &peers() const;
};

#endif /* TAILCTL_STATUS_H */
