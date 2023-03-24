// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_APP_H
#define TAILCTL_APP_H

#include "peer_model.h"
#include "status.h"
#include "tailctlconfig.h"
#include "tailscale_client.h"

#include <QObject>
#include <QProcess>
#include <QSystemTrayIcon>

class QQuickWindow;

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TailctlConfig *config READ config NOTIFY configChanged)
    Q_PROPERTY(Status *status READ status NOTIFY statusChanged)
    Q_PROPERTY(Peer *peerDetails READ peerDetails NOTIFY peerDetailsChanged)
    Q_PROPERTY(PeerModel *peerModel READ peerModel NOTIFY peerModelChanged)

private:
    TailctlConfig *m_config;
    TailscaleClient m_client;
    Status m_status;
    Peer m_peer_details;
    PeerModel m_peer_model;

    QSystemTrayIcon m_tray_icon;

signals:
    void configChanged();
    void statusChanged();
    void peerDetailsChanged();
    void peerModelChanged();

public slots:
    Q_INVOKABLE void updateTrayMenu();
    Q_INVOKABLE void toggleTaildrop();

public:
    App(QObject *parent = nullptr);

    TailctlConfig *config();
    Status *status();
    Peer *peerDetails();
    PeerModel *peerModel();

    // Restore current window geometry
    Q_INVOKABLE void restoreWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;
    // Save current window geometry
    Q_INVOKABLE void saveWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;

    Q_INVOKABLE void setPeerDetails(const QString &id);

    Q_INVOKABLE void setClipboardText(const QString &text);
};

#endif /* TAILCTL_APP_H */
