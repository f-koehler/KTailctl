// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tray_icon.hpp"
#include "ktailctlconfig.h"
#include "taildrop_sender.hpp"
#include "util.hpp"

#include <QClipboard>
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
    setContextMenu(new QMenu());

    QObject::connect(contextMenu(), &QMenu::aboutToShow, this, &TrayIcon::regenerate);
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

void TrayIcon::addToggleAction(QMenu *menu)
{
    QAction *actionToggle = menu->addAction("Toggle", [this]() {
        mTailscale->toggle();
    });
    if (mTailscale->backendState() == "Running") {
        actionToggle->setChecked(true);
        actionToggle->setText("Stop Tailscale");
        actionToggle->setIcon(QIcon::fromTheme(QStringLiteral("process-stop")));
    } else {
        actionToggle->setChecked(false);
        actionToggle->setText("Start Tailscale");
        actionToggle->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    }
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
void TrayIcon::addPeerMenu(QMenu *menu)
{
    QMenu *peerMenu = menu->addMenu(QIcon::fromTheme("applications-network"), "Peers");
    const PeerModel *model = mTailscale->peerModel();
    const int numPeers = model->rowCount({});

    auto createCopyAction = [](QMenu *menu, const QString &text) {
        auto *action = menu->addAction(QIcon::fromTheme("edit-copy"), text, [text]() {
            setClipboardText(text);
        });
        return action;
    };

    for (int i = 0; i < numPeers; ++i) {
        const QModelIndex index = model->index(i, 0);
        const QString dnsName = index.data(PeerModel::DnsNameRole).toString();
        if (index.data(PeerModel::IsMullvadRole).toBool()) {
            continue;
        }

        QMenu *submenu = peerMenu->addMenu(loadOsIcon(index.data(PeerModel::OsRole).toString()), dnsName);
        createCopyAction(submenu, dnsName);
        const QStringList ips = index.data(PeerModel::TailscaleIpsRole).toStringList();
        for (const QString &ip : ips) {
            createCopyAction(submenu, ip);
        }

        if (!mTailscale->isOperator()) {
            continue;
        }

        submenu->addSection("Taildrop Send");
        submenu->addAction(QIcon::fromTheme(QStringLiteral("document-send")), "Send file(s)", [&dnsName]() {
            TaildropSendJob::selectAndSendFiles(dnsName);
        });

        if (index.data(PeerModel::IsCurrentExitNodeRole).toBool()) {
            submenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), "Unset exit node", [this]() {
                mTailscale->unsetExitNode();
            });
        } else if (index.data(PeerModel::IsExitNodeRole).toBool()) {
            submenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), "Set as exit node", [this, &index]() {
                mTailscale->setExitNode(index.data(PeerModel::TailscaleIpsRole).toStringList().front());
            });
        }
    }
}

void TrayIcon::regenerate()
{
    if (!isVisible()) {
        return;
    }

    QMenu *menu = contextMenu();
    menu->clear();

    updateIcon();

    if (mWindow != nullptr) {
        menu->addAction(QIcon::fromTheme("window"), "Open", this, [this]() {
            mWindow->show();
        });
        menu->addSeparator();
    }

    if (mTailscale->success()) {
        addPeerMenu(menu);
        if (mTailscale->isOperator()) {
            addExitNodeMenu(menu);
            addToggleAction(menu);
        }
    }

    menu->addSeparator();
    menu->addAction(QIcon::fromTheme("application-exit"), "Quit", [this]() {
        emit quitClicked();
    });
    setContextMenu(menu);
}

void TrayIcon::updateIcon()
{
    setIcon(QIcon(QString(":/icons/%1-%2")
                      .arg((mTailscale->backendState() == "Running") ? QStringLiteral("online") : QStringLiteral("offline"), KTailctlConfig::trayIconTheme())));
}

void TrayIcon::setWindow(QQuickWindow *window)
{
    mWindow = window;
}
