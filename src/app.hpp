// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_APP_HPP
#define KTAILCTL_APP_HPP

#include "ktailctlconfig.h"
#include "peer_data.hpp"
#include "peer_model.hpp"
#include "tailscale.hpp"
#include "tray_icon.hpp"

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
    Q_PROPERTY(QSortFilterProxyModel *peerModel READ peerModel CONSTANT)
    Q_PROPERTY(QSortFilterProxyModel *mullvadNodesForCountryModel READ mullvadNodesForCountryModel CONSTANT)
    Q_PROPERTY(PeerData peerDetails READ peerDetails NOTIFY peerDetailsChanged)
    // Q_PROPERTY(TrayIcon *trayIcon READ trayIcon CONSTANT)

private:
    KTailctlConfig *mConfig;
    QSortFilterProxyModel *mPeerModel;
    QSortFilterProxyModel *mMullvadNodesForCountryModel;
    bool mFilterInitialized = false;

    TrayIcon *mTrayIcon;
    PeerData mPeerDetails;

public slots:
    void refreshDetails();
    static void quitApp();

signals:
    void peerDetailsChanged(const PeerData &peerDetails);

public:
    explicit App(QObject *parent = nullptr);
    virtual ~App() = default;

    KTailctlConfig *config();
    QSortFilterProxyModel *peerModel();
    QSortFilterProxyModel *mullvadNodesForCountryModel();
    const PeerData &peerDetails() const;
    TrayIcon *trayIcon();

    // Restore current window geometry
    Q_INVOKABLE static void restoreWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main"));
    // Save current window geometry
    Q_INVOKABLE static void saveWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main"));

    Q_INVOKABLE void setPeerDetails(const QString &id);
};

#endif /* KTAILCTL_APP_HPP */
