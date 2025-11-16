// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "app.hpp"
#include "logging_app.hpp"
#include "taildrop_receiver.hpp"
#include <KSharedConfig>
#include <KWindowConfig>
#include <QClipboard>
#include <QGuiApplication>
#include <QList>
#include <QMenu>
#include <QQuickWindow>

#include <functional>

App::App(QObject *parent)
    : QObject(parent)
    , mConfig(KTailctlConfig::self())
    , mPeerModel(new QSortFilterProxyModel(this))
    , mMullvadNodesForCountryModel(new QSortFilterProxyModel(this))
{
    QObject::connect(Tailscale::instance(), &Tailscale::statusRefreshed, this, &App::refreshDetails);

    mPeerModel->setSourceModel(Tailscale::instance()->peerModel());
    mPeerModel->setFilterRole(PeerModel::DnsNameRole);
    mMullvadNodesForCountryModel->setSourceModel(Tailscale::instance()->mullvadNodeModel());
    mMullvadNodesForCountryModel->setFilterRole(PeerModel::CountryCodeRole);

    if (KTailctlConfig::peerFilter() == "UNINITIALIZED") {
        Tailscale::instance()->refreshStatus();
        const QString domain = Tailscale::instance()->self().mDnsName.section('.', 1);
        mPeerModel->setFilterRegularExpression(domain);
        KTailctlConfig::setPeerFilter(domain);
        mConfig->save();
    }
}

KTailctlConfig *App::config()
{
    return mConfig;
}
QSortFilterProxyModel *App::peerModel()
{
    return mPeerModel;
}
QSortFilterProxyModel *App::mullvadNodesForCountryModel()
{
    return mMullvadNodesForCountryModel;
}
const PeerData &App::peerDetails() const
{
    return mPeerDetails;
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
    const QVector<PeerData> &peers = Tailscale::instance()->peerModel()->peers();
    const QVector<PeerData>::const_iterator position = std::find_if(peers.begin(), peers.end(), [&id](const PeerData &peer) {
        return peer.mId == id;
    });
    if (position == peers.end()) {
        qCWarning(Logging::App) << "Peer" << id << "not found";
        return;
    }
    if (*position != mPeerDetails) {
        mPeerDetails = *position;
        emit peerDetailsChanged(mPeerDetails);
    }
}

void App::quitApp()
{
    TaildropReceiver::self()->quit();
    qApp->quit();
}

void App::refreshDetails()
{
    if (std::find_if(Tailscale::instance()->peerModel()->peers().begin(),
                     Tailscale::instance()->peerModel()->peers().end(),
                     [this](const PeerData &peer) {
                         if (peer.mId == mPeerDetails.mId) {
                             if (peer != mPeerDetails) {
                                 mPeerDetails = peer;
                                 emit peerDetailsChanged(peer);
                             }
                             return true;
                         }
                         return false;
                     })
        != Tailscale::instance()->peerModel()->peers().end()) {
        return;
    }
    if (!Tailscale::instance()->peerModel()->peers().isEmpty()) {
        mPeerDetails = Tailscale::instance()->peerModel()->peers().front();
    }
    emit peerDetailsChanged(mPeerDetails);
}
