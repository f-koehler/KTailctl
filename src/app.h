// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_APP_H
#define KTAILCTL_APP_H

#include "ktailctlconfig.h"
#include "peer_model.h"
#include "tailscale.h"
#include "tray_icon.h"

#include <QLoggingCategory>
#include <QObject>
#include <QQuickWindow>
#include <QSortFilterProxyModel>

Q_DECLARE_LOGGING_CATEGORY(logcat_app)

class QQuickWindow;

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(KTailctlConfig *config READ config CONSTANT)
    Q_PROPERTY(Peer *peerDetails READ peerDetails NOTIFY peerDetailsChanged)
    Q_PROPERTY(QSortFilterProxyModel *peerModel READ peerModel CONSTANT)
    // Q_PROPERTY(TrayIcon *trayIcon READ trayIcon CONSTANT)

private:
    KTailctlConfig *mConfig;
    Peer *mPeerDetails;
    QSortFilterProxyModel *mPeerProxyModel;
    bool mFilterInitialized = false;

    TrayIcon *mTrayIcon;

signals:
    void peerDetailsChanged();

public slots:
    static void quitApp();

public:
    explicit App(QObject *parent = nullptr);
    virtual ~App() = default;

    KTailctlConfig *config();
    Peer *peerDetails();
    QSortFilterProxyModel *peerModel();
    TrayIcon *trayIcon();

    // Restore current window geometry
    Q_INVOKABLE static void restoreWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main"));
    // Save current window geometry
    Q_INVOKABLE static void saveWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main"));

    Q_INVOKABLE void setPeerDetails(const QString &id);
};

#endif /* KTAILCTL_APP_H */
