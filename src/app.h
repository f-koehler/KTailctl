// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_APP_H
#define TAILCTL_APP_H

#include "ktailctlconfig.h"
#include "notifier.h"
#include "peer_model.h"
#include "status.h"
#include "taildrop_process.h"
#include "tailscale.h"
#include "tray_icon.h"

#include <QObject>
#include <QProcess>
#include <QQuickWindow>

class QQuickWindow;

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Tailscale *tailscale READ tailscale NOTIFY tailscaleChanged)
    Q_PROPERTY(KTailctlConfig *config READ config NOTIFY configChanged)
    Q_PROPERTY(Peer *peerDetails READ peerDetails NOTIFY peerDetailsChanged)
    Q_PROPERTY(PeerModel *peerModel READ peerModel NOTIFY peerModelChanged)

private:
    Tailscale *mTailscale;
    KTailctlConfig *mConfig;
    TaildropProcess mTaildropProcess;
    Peer mPeerDetails;
    PeerModel mPeerModel;
    Notifier *mNotifier;

    TrayIcon *mTrayIcon;

signals:
    void tailscaleChanged();
    void configChanged();
    void peerDetailsChanged();
    void peerModelChanged();

public:
    App(Tailscale *tailscale, QObject *parent = nullptr);

    Tailscale *tailscale();
    KTailctlConfig *config();
    Peer *peerDetails();
    PeerModel *peerModel();
    TrayIcon *trayIcon();

    // Restore current window geometry
    Q_INVOKABLE void restoreWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;
    // Save current window geometry
    Q_INVOKABLE void saveWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;

    Q_INVOKABLE void setPeerDetails(const QString &id);
};

#endif /* TAILCTL_APP_H */
