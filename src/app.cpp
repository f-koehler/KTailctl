// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "app.h"
#include <KSharedConfig>
#include <KWindowConfig>
#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>
#include <QQuickWindow>

#include <functional>

QString strategyToString(const TailctlConfig::EnumTaildropStrategy::type &strategy)
{
    switch (strategy) {
    case TailctlConfig::EnumTaildropStrategy::Overwrite:
        return "overwrite";
    case TailctlConfig::EnumTaildropStrategy::Skip:
        return "skip";
    default:
        return "rename";
    }
}

App::App(QObject *parent)
    : QObject(parent)
    , mConfig(TailctlConfig::self())
    , mTaildropProcess(mConfig->tailscaleExecutable(),
                       mConfig->taildropEnabled(),
                       mConfig->taildropDirectory(),
                       strategyToString(mConfig->taildropStrategy()),
                       this)
    , mStatus(new Status(this))
    , mTrayIcon(new TrayIcon(mStatus, this))
{
    QObject::connect(mConfig, &TailctlConfig::tailscaleExecutableChanged, [this]() {
        this->mTaildropProcess.setExecutable(mConfig->tailscaleExecutable());
    });
    QObject::connect(mConfig, &TailctlConfig::taildropEnabledChanged, [this]() {
        this->mTaildropProcess.setEnabled(mConfig->taildropEnabled());
    });
    QObject::connect(mConfig, &TailctlConfig::taildropDirectoryChanged, [this]() {
        this->mTaildropProcess.setDirectory(mConfig->taildropDirectory());
    });
    QObject::connect(mConfig, &TailctlConfig::taildropStrategyChanged, [this]() {
        this->mTaildropProcess.setStrategy(strategyToString(mConfig->taildropStrategy()));
    });

    QObject::connect(mStatus, &Status::peersChanged, &mPeerModel, &PeerModel::updatePeers);
    QObject::connect(mStatus, &Status::refreshed, &mPeerDetails, &Peer::updateFromStatus);
}

TailctlConfig *App::config()
{
    return mConfig;
}
Status *App::status()
{
    return mStatus;
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
    if (mStatus->self()->id() == id) {
        mPeerDetails = *mStatus->self();
        emit peerDetailsChanged();
    } else {
        auto pos = std::find_if(mStatus->peers().begin(), mStatus->peers().end(), [&id](Peer *peer) {
            return peer->id() == id;
        });
        if (pos == mStatus->peers().end()) {
            qWarning() << "Peer" << id << "not found";
            return;
        }
        if (mPeerDetails.setTo(*pos)) {
            emit peerDetailsChanged();
        }
    }
}