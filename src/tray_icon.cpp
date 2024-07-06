// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tray_icon.hpp"
#include "ktailctlconfig.h"
#include "taildrop_sender.hpp"
#include "util.hpp"

#include <QClipboard>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QGuiApplication>
#include <QIcon>
#include <QMap>
#include <QMenu>
#include <QMessageBox>

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
    , mTailscale(Tailscale::instance())
    , mConfig(KTailctlConfig::self())
{
    updateIcon();

    mContextMenu = new QMenu();
    setContextMenu(mContextMenu);

    mOpenAction = mContextMenu->addAction(QIcon::fromTheme(QStringLiteral("window")), QStringLiteral("Open"), [this]() {
        mWindow->show();
    });
    mContextMenu->addSeparator();
    mToggleAction = mContextMenu->addAction(QStringLiteral("Toggle"), [this]() {
        mTailscale->toggle();
    });
    mPeerMenu = mContextMenu->addMenu(QIcon::fromTheme(QStringLiteral("applications-network")), QStringLiteral("Peers"));
    mExitNodeMenu = mContextMenu->addMenu(QIcon::fromTheme(QStringLiteral("internet-services")), QStringLiteral("Exit Nodes"));
    mUnsetAction = mExitNodeMenu->addAction(QIcon::fromTheme(QStringLiteral("dialog-cancel")), QStringLiteral("Unset"));
    mSuggestedAction = mExitNodeMenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Use suggested"));
    mExitNodeMenu->addSeparator();
    mMullvadMenu = mExitNodeMenu->addMenu(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Mullvad"));
    mSelfHostedMenu = mExitNodeMenu->addMenu(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Self-hosted"));
    mContextMenu->addSeparator();
    mQuitAction = mContextMenu->addAction(QIcon::fromTheme(QStringLiteral("application-exit")), QStringLiteral("Quit"), [this]() {
        emit quitClicked();
    });

    connect(mTailscale, &Tailscale::backendStateChanged, [this]() {
        updateIcon();
        if (mTailscale->backendState() == QStringLiteral("Running")) {
            mToggleAction->setChecked(true);
            mToggleAction->setText(QStringLiteral("Stop Tailscale"));
            mToggleAction->setIcon(QIcon::fromTheme(QStringLiteral("process-stop")));
            buildPeerMenu(mPeerMenu);
        } else {
            mToggleAction->setChecked(false);
            mToggleAction->setText(QStringLiteral("Start Tailscale"));
            mToggleAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
        }
    });
    connect(mTailscale, &Tailscale::successChanged, [this]() {
        if (mTailscale->success()) {
            buildPeerMenu(mPeerMenu);
            mPeerMenu->setEnabled(true);
            if (mTailscale->isOperator()) {
                mToggleAction->setEnabled(true);
                mExitNodeMenu->setEnabled(true);
            }
        } else {
            mPeerMenu->setEnabled(false);
            mToggleAction->setEnabled(false);
            mExitNodeMenu->setEnabled(false);
        }
    });
    connect(mTailscale, &Tailscale::isOperatorChanged, [this]() {
        if (mTailscale->isOperator()) {
            mToggleAction->setEnabled(true);
            mPeerMenu->setEnabled(true);
            mExitNodeMenu->setEnabled(true);
        } else {
            mToggleAction->setEnabled(false);
            mPeerMenu->setEnabled(false);
            mExitNodeMenu->setEnabled(false);
        }
    });

    QObject::connect(mConfig, &KTailctlConfig::trayIconThemeChanged, this, &TrayIcon::updateIcon);
    QObject::connect(this, &QSystemTrayIcon::activated, [this](const QSystemTrayIcon::ActivationReason &reason) {
        switch (reason) {
        case QSystemTrayIcon::ActivationReason::Trigger:
        case QSystemTrayIcon::ActivationReason::DoubleClick:
        case QSystemTrayIcon::ActivationReason::MiddleClick:
            if (mWindow == nullptr) {
                return;
            }
            if (mWindow->isVisible()) {
                mWindow->hide();
            } else {
                mWindow->show();
            }
            break;
        default:
            break;
        };
    });

    show();
}

void TrayIcon::addExitNodeMenu(QMenu *menu)
{
    bool hasExitNodes = mTailscale->exitNodeModel()->rowCount() > 0;
    bool hasMullvadNodes = mTailscale->mullvadNodeModel()->rowCount() > 0;
    QMenu *menuExitNodes = nullptr;
    if (!hasExitNodes && !hasMullvadNodes) {
        return;
    }
    menuExitNodes = menu->addMenu(QIcon::fromTheme("internet-services"), "Exit Nodes");
    if (mTailscale->hasCurrentExitNode()) {
        menuExitNodes->addAction(QIcon::fromTheme("dialog-cancel"), QString("Unset %1").arg(mTailscale->currentExitNode().mDnsName), [this]() {
            mTailscale->unsetExitNode();
        });
    }
    if (mTailscale->hasSuggestedExitNode()) {
        const QIcon icon = (mTailscale->suggestedExitNode().mCountryCode.isEmpty())
            ? QIcon::fromTheme("network-vpn")
            : QIcon(QString(":/country-flags/country-flag-%1").arg(mTailscale->suggestedExitNode().mCountryCode.toLower()));
        menuExitNodes->addAction(icon, QString("Use suggested: %1").arg(mTailscale->suggestedExitNode().mDnsName), [this]() {
            mTailscale->setExitNode(mTailscale->suggestedExitNode().mTailscaleIps.front());
        });
    }
    if (hasMullvadNodes) {
        addMullvadMenu(menuExitNodes);
        if (hasExitNodes) {
            menuExitNodes->addSeparator();
        }
    }
    if (hasExitNodes) {
        addExitNodeActions(menuExitNodes);
    }
}

void TrayIcon::addMullvadMenu(QMenu *menu)
{
    QMenu *mullvadMenu = menu->addMenu(QIcon::fromTheme("network-vpn"), "Mullvad");
    MullvadCountryModel *mullvadCountryModel = mTailscale->mullvadCountryModel();

    // Create a menu for each country
    const int numCountries = mullvadCountryModel->rowCount({});
    QMap<QString, QMenu *> countryMenus;

    for (int i = 0; i < numCountries; ++i) {
        const QModelIndex index = mullvadCountryModel->index(i, 0);
        const QString countryCode = index.data(MullvadCountryModel::CountryCode).toString().toLower();
        countryMenus.insert(
            countryCode,
            mullvadMenu->addMenu(QIcon(QString(":/country-flags/country-flag-%1").arg(countryCode)), index.data(MullvadCountryModel::CountryName).toString()));
    }

    // Add nodes to the country menus
    // The nodes are sorted by country code and then by DNS name
    MullvadNodeModel *mullvadNodeModel = mTailscale->mullvadNodeModel();
    const int numNodes = mullvadNodeModel->rowCount({});
    for (int i = 0; i < numNodes; ++i) {
        const QModelIndex index = mullvadNodeModel->index(i, 0);
        const QString countryCode = index.data(PeerModel::CountryCodeRole).toString().toLower();
        countryMenus[countryCode]->addAction(QIcon(QString(":/country-flags/country-flag-%1").arg(countryCode)),
                                             index.data(PeerModel::DnsNameRole).toString(),
                                             [this, &index]() {
                                                 mTailscale->setExitNode(index.data(PeerModel::TailscaleIpsRole).toStringList().front());
                                             });
    }
}
void TrayIcon::addExitNodeActions(QMenu *menu)
{
    const int numNodes = mTailscale->exitNodeModel()->rowCount();
    for (int i = 0; i < numNodes; ++i) {
        const QModelIndex index = mTailscale->exitNodeModel()->index(i, 0);
        menu->addAction(QIcon::fromTheme("network-vpn"), index.data(PeerModel::DnsNameRole).toString(), [this, &index]() {
            mTailscale->setExitNode(index.data(PeerModel::TailscaleIpsRole).toStringList().front());
        });
    }
}
void TrayIcon::buildPeerMenu(QMenu *menu)
{
    menu->clear();
    const QVector<PeerData> &peers = mTailscale->peerModel()->peers();

    auto createCopyAction = [](QMenu *menu, const QString &text) {
        QAction *action = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), text, [text]() {
            setClipboardText(text);
        });
        return action;
    };

    for (const PeerData &peer : peers) {
        if (peer.mIsMullvad) {
            continue;
        }
        QMenu *submenu = menu->addMenu(loadOsIcon(peer.mOs), peer.mDnsName);
        createCopyAction(submenu, peer.mDnsName);
        for (const QString &ip : peer.mTailscaleIps) {
            createCopyAction(submenu, ip);
        }

        if (!mTailscale->isOperator()) {
            continue;
        }

        submenu->addSection(QStringLiteral("Taildrop Send"));
        submenu->addAction(QIcon::fromTheme(QStringLiteral("document-send")), QStringLiteral("Send file(s)"), [&peer]() {
            TaildropSendJob::selectAndSendFiles(peer.mDnsName);
        });

        if (peer.mIsCurrentExitNode) {
            submenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Unset exit node"), [this]() {
                mTailscale->unsetExitNode();
            });
        } else if (peer.mIsExitNode) {
            submenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Set as exit node"), [this, &peer]() {
                mTailscale->setExitNode(peer.mTailscaleIps.front());
            });
        }
    }
}

void TrayIcon::updateIcon()
{
    setIcon(QIcon(QString(":/icons/%1-%2")
                      .arg((mTailscale->backendState() == "Running") ? QStringLiteral("online") : QStringLiteral("offline"), KTailctlConfig::trayIconTheme())));
}
void TrayIcon::regenerate()
{
    buildPeerMenu(mPeerMenu);
}

void TrayIcon::setWindow(QQuickWindow *window)
{
    mWindow = window;
    mOpenAction->setVisible(mWindow != nullptr);
}
