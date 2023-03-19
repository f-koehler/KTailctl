// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_APP_H
#define TAILCTL_APP_H

#include "peer_model.h"
#include "status.h"

#include <QObject>

class QQuickWindow;

class App : public QObject {
  Q_OBJECT
  Q_PROPERTY(Status *status READ status NOTIFY statusChanged);
  Q_PROPERTY(Peer *peerDetails READ peerDetails NOTIFY peerDetailsChanged);
  Q_PROPERTY(PeerModel *peerModel READ peerModel NOTIFY peerModelChanged);

private:
  Status m_status;
  Peer m_peer_details;
  PeerModel m_peer_model;

signals:

  void statusChanged();
  void peerDetailsChanged();
  void peerModelChanged();

public:
  App(QObject *parent = nullptr);
  Status *status();
  Peer *peerDetails();
  PeerModel *peerModel();

  // Restore current window geometry
  Q_INVOKABLE void
  restoreWindowGeometry(QQuickWindow *window,
                        const QString &group = QStringLiteral("main")) const;
  // Save current window geometry
  Q_INVOKABLE void
  saveWindowGeometry(QQuickWindow *window,
                     const QString &group = QStringLiteral("main")) const;

  Q_INVOKABLE void setPeerDetails(const QString &id);
};

#endif /* TAILCTL_APP_H */
