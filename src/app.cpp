// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "app.h"
#include <KSharedConfig>
#include <KWindowConfig>
#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>
#include <QQuickWindow>

#include <functional>

Q_LOGGING_CATEGORY(logcat_app, "org.fkoehler.KTailctl.App")

App::App(Tailscale *tailscale, QObject *parent)
    : QObject(parent)
    , mTailscale(tailscale)
    , mConfig(KTailctlConfig::self())
    , mPeerDetails(new Peer(this))
    , mPeerModel(new PeerModel(this))
    , mPeerProxyModel(new QSortFilterProxyModel(this))
    , mExitNodeModel(new ExitNodeModel(this))
    , mTrayIcon(new TrayIcon(tailscale, this))
{
    mTailscale->setParent(this);

    QObject::connect(tailscale->status(), &Status::refreshed, mPeerModel, &PeerModel::updatePeers);
    QObject::connect(tailscale->status(), &Status::refreshed, mExitNodeModel, &ExitNodeModel::updatePeers);
    // QObject::connect(tailscale->status(), &Status::refreshed, &mPeerDetails, &Peer::updateFromStatus);
    QObject::connect(tailscale->status(), &Status::backendStateChanged, mTrayIcon, &TrayIcon::regenerate);
    QObject::connect(mTrayIcon, &TrayIcon::quitClicked, this, &App::quitApp);

    if (KTailctlConfig::peerFilter() == "UNINITIALIZED") {
        tailscale->status()->refresh();
        const auto domain = mTailscale->status()->self()->dnsName().section('.', 1);
        mPeerProxyModel->setFilterRegularExpression(domain);
        KTailctlConfig::setPeerFilter(domain);
        mConfig->save();
    }
    mPeerProxyModel->setSourceModel(mPeerModel);
    mPeerProxyModel->setFilterRole(PeerModel::DnsNameRole);
}

Tailscale *App::tailscale()
{
    return mTailscale;
}
KTailctlConfig *App::config()
{
    return mConfig;
}
Peer *App::peerDetails()
{
    return mPeerDetails;
}
QSortFilterProxyModel *App::peerModel()
{
    return mPeerProxyModel;
}
ExitNodeModel *App::exitNodeModel()
{
    return mExitNodeModel;
}
TrayIcon *App::trayIcon()
{
    return mTrayIcon;
}

void App::restoreWindowGeometry(QQuickWindow *window, const QString &group)
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup const windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::restoreWindowSize(window, windowGroup);
    KWindowConfig::restoreWindowPosition(window, windowGroup);
}

void App::saveWindowGeometry(QQuickWindow *window, const QString &group)
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::saveWindowPosition(window, windowGroup);
    KWindowConfig::saveWindowSize(window, windowGroup);
    dataResource.sync();
}

void App::setPeerDetails(const QString &id)
{
    auto *pos = std::find_if(mTailscale->status()->peers().begin(), mTailscale->status()->peers().end(), [&id](Peer *peer) {
        return peer->id() == id;
    });
    if (pos == mTailscale->status()->peers().end()) {
        qCWarning(logcat_app) << "Peer" << id << "not found";
        return;
    }
    PeerData data = (*pos)->peerData();
    mPeerDetails->update(data);
}

void App::quitApp()
{
    TaildropReceiver::self()->quit();
    qApp->quit();
}
