// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_APP_H
#define KTAILCTL_APP_H

#include "exit_node_model.h"
#include "ktailctlconfig.h"
#include "peer_model.h"
#include "status.h"
#include "tailscale.h"
#include "tray_icon.h"

#include <QObject>
#include <QQuickWindow>
#include <QSortFilterProxyModel>

class QQuickWindow;

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Tailscale *tailscale READ tailscale CONSTANT)
    Q_PROPERTY(KTailctlConfig *config READ config CONSTANT)
    Q_PROPERTY(Peer *peerDetails READ peerDetails NOTIFY peerDetailsChanged)
    Q_PROPERTY(QSortFilterProxyModel *peerModel READ peerModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel *exitNodeModel READ exitNodeModel CONSTANT)
    // Q_PROPERTY(TrayIcon *trayIcon READ trayIcon CONSTANT)

private:
    Tailscale *mTailscale;
    KTailctlConfig *mConfig;
    Peer *mPeerDetails;
    PeerModel *mPeerModel;
    QSortFilterProxyModel *mPeerProxyModel;
    ExitNodeModel *mExitNodeModel;
    bool mFilterInitialized = false;

    TrayIcon *mTrayIcon;

signals:
    void peerDetailsChanged();

public slots:
    static void quitApp();

public:
    explicit App(Tailscale *tailscale, QObject *parent = nullptr);
    virtual ~App() = default;

    Tailscale *tailscale();
    KTailctlConfig *config();
    Peer *peerDetails();
    QSortFilterProxyModel *peerModel();
    ExitNodeModel *exitNodeModel();
    TrayIcon *trayIcon();

    // Restore current window geometry
    Q_INVOKABLE static void restoreWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main"));
    // Save current window geometry
    Q_INVOKABLE static void saveWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main"));

    Q_INVOKABLE void setPeerDetails(const QString &id);
};

#endif /* KTAILCTL_APP_H */
