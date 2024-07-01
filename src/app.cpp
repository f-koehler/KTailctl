// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "app.h"
#include "taildrop_receiver.h"
#include <KSharedConfig>
#include <KWindowConfig>
#include <QClipboard>
#include <QGuiApplication>
#include <QList>
#include <QMenu>
#include <QQuickWindow>

#include <functional>

Q_LOGGING_CATEGORY(logcat_app, "org.fkoehler.KTailctl.App")

App::App(QObject *parent)
    : QObject(parent)
    , mConfig(KTailctlConfig::self())
    , mPeerDetails(new Peer(this))
    , mPeerProxyModel(new QSortFilterProxyModel(this))
    , mTrayIcon(new TrayIcon(this))
{
    // QObject::connect(tailscale->status(), &Status::refreshed, &mPeerDetails, &Peer::updateFromStatus);
    QObject::connect(Tailscale::getInstance(), &Tailscale::backendStateChanged, mTrayIcon, &TrayIcon::regenerate);
    QObject::connect(mTrayIcon, &TrayIcon::quitClicked, this, &App::quitApp);

    if (KTailctlConfig::peerFilter() == "UNINITIALIZED") {
        Tailscale::getInstance()->refresh();
        const auto domain = Tailscale::getInstance()->self()->dnsName().section('.', 1);
        mPeerProxyModel->setFilterRegularExpression(domain);
        KTailctlConfig::setPeerFilter(domain);
        mConfig->save();
    }
    mPeerProxyModel->setSourceModel(Tailscale::getInstance()->peerModel());
    mPeerProxyModel->setFilterRole(PeerModel::DnsNameRole);
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
    auto pos = std::find_if(Tailscale::getInstance()->peers().begin(), Tailscale::getInstance()->peers().end(), [&id](Peer *peer) {
        return peer->id() == id;
    });
    if (pos == Tailscale::getInstance()->peers().end()) {
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
