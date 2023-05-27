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

QString strategyToString(const KTailctlConfig::EnumTaildropStrategy::type &strategy)
{
    switch (strategy) {
    case KTailctlConfig::EnumTaildropStrategy::Overwrite:
        return "overwrite";
    case KTailctlConfig::EnumTaildropStrategy::Skip:
        return "skip";
    default:
        return "rename";
    }
}

App::App(Tailscale *tailscale, QObject *parent)
    : QObject(parent)
    , mTailscale(tailscale)
    , mConfig(KTailctlConfig::self())
    , mTaildropReceiver(mConfig->taildropEnabled(), mConfig->taildropDirectory(), strategyToString(mConfig->taildropStrategy()), this)
    , mTrayIcon(new TrayIcon(tailscale, this))
{
    QObject::connect(mConfig, &KTailctlConfig::taildropEnabledChanged, [this]() {
        this->mTaildropReceiver.setEnabled(mConfig->taildropEnabled());
    });
    QObject::connect(mConfig, &KTailctlConfig::taildropDirectoryChanged, [this]() {
        this->mTaildropReceiver.setDirectory(mConfig->taildropDirectory());
    });
    QObject::connect(mConfig, &KTailctlConfig::taildropStrategyChanged, [this]() {
        this->mTaildropReceiver.setStrategy(strategyToString(mConfig->taildropStrategy()));
    });

    QObject::connect(tailscale->status(), &Status::peersChanged, &mPeerModel, &PeerModel::updatePeers);
    QObject::connect(tailscale->status(), &Status::refreshed, &mPeerDetails, &Peer::updateFromStatus);
    QObject::connect(tailscale->status(), &Status::backendStateChanged, mTrayIcon, &TrayIcon::regenerate);
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
    return &mPeerDetails;
}
PeerModel *App::peerModel()
{
    return &mPeerModel;
}
TrayIcon *App::trayIcon()
{
    return mTrayIcon;
}

void App::restoreWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::restoreWindowSize(window, windowGroup);
    KWindowConfig::restoreWindowPosition(window, windowGroup);
}

void App::saveWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::saveWindowPosition(window, windowGroup);
    KWindowConfig::saveWindowSize(window, windowGroup);
    dataResource.sync();
}

void App::setPeerDetails(const QString &id)
{
    if (mTailscale->status()->self()->id() == id) {
        mPeerDetails = *mTailscale->status()->self();
        emit peerDetailsChanged();
    } else {
        auto pos = std::find_if(mTailscale->status()->peers().begin(), mTailscale->status()->peers().end(), [&id](Peer *peer) {
            return peer->id() == id;
        });
        if (pos == mTailscale->status()->peers().end()) {
            qWarning() << "Peer" << id << "not found";
            return;
        }
        if (mPeerDetails.setTo(*pos)) {
            emit peerDetailsChanged();
        }
    }
}