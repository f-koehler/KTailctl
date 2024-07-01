// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tray_icon.h"
#include "ktailctlconfig.h"
#include "taildrop_sender.h"
#include "util.h"

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
    QObject::connect(mConfig, &KTailctlConfig::trayIconStyleChanged, this, &TrayIcon::updateIcon);
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

    if (mTailscale->isOperator() && mTailscale->success()) {
        auto *actionToggle = menu->addAction("Toggle", [this]() {
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

        const QVector<Peer *> &exitNodes = mTailscale->exitNodes();
        if (!exitNodes.empty()) {
            auto *menuExitNodes = menu->addMenu(QIcon::fromTheme("internet-services"), "Exit Nodes");

            if (mTailscale->exitNode() != nullptr) {
                menuExitNodes->addAction(QIcon::fromTheme("dialog-cancel"), QString("Unset %1").arg(mTailscale->exitNode()->dnsName()), [this]() {
                    mTailscale->unsetExitNode();
                });
                menuExitNodes->addSeparator();
            }

            if (mTailscale->suggestedExitNode() != nullptr) {
                menuExitNodes->addAction(QIcon::fromTheme("network-vpn"), QString("Suggested: %1").arg(mTailscale->suggestedExitNode()->hostName()), [this]() {
                    mTailscale->setExitNode(mTailscale->suggestedExitNode());
                });
            }

            QMenu *menuMullvadNodes = menuExitNodes->addMenu(QIcon::fromTheme("network-vpn"), "Mullvad Exit Nodes");
            QMap<QString, QMenu *> mullvadSubMenus;
            for (Peer *node : mTailscale->exitNodes()) {
                if (!node->isMullvad()) {
                    menuExitNodes->addAction(loadOsIcon(node->os()), node->hostName(), [this, node]() {
                        mTailscale->setExitNode(node);
                    });
                    continue;
                }
                if (node->location() == nullptr) {
                    continue;
                }
                const QString &countryCode = node->location()->countryCode();
                auto menuPosition = mullvadSubMenus.lowerBound(countryCode);
                if ((menuPosition == mullvadSubMenus.end()) || menuPosition.key() != countryCode) {
                    QMenu *menu = new QMenu(countryCode, menuMullvadNodes);
                    menu->setIcon(QIcon(QString(":/country-flags/%1").arg(countryCode.toLower())));
                    menuPosition = mullvadSubMenus.insert(countryCode, menu);
                }
                menuPosition.value()->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), node->hostName(), [this, node]() {
                    mTailscale->setExitNode(node);
                });
            }

            for (auto it = mullvadSubMenus.begin(); it != mullvadSubMenus.end(); ++it) {
                menuMullvadNodes->addMenu(it.value());
            }
        }
        menu->addSeparator();
    }

    auto create_action = [](QMenu *menu, const QString &text) {
        auto *action = menu->addAction(QIcon::fromTheme("edit-copy"), text, [text]() {
            setClipboardText(text);
        });
        return action;
    };

    if (mTailscale->success()) {
        auto *peer_menu = menu->addMenu(QIcon::fromTheme("applications-network"), "Peers");
        for (auto *peer : mTailscale->peers()) {
            if (peer->isMullvad()) {
                continue;
            }
            auto *submenu = peer_menu->addMenu(loadOsIcon(peer->os()), peer->hostName());
            create_action(submenu, peer->dnsName());
            for (const auto &address : peer->tailscaleIps()) {
                create_action(submenu, address);
            }
            submenu->addSection("Statistics");

            // auto *statsDown = mTailscale->statistics()->speedDown(peer->id());
            // auto *statsUp = mTailscale->statistics()->speedUp(peer->id());
            // auto *actionDown = submenu->addAction(QIcon::fromTheme("cloud-download"), formatSpeedHumanReadable(statsDown->average()));
            // auto *actionUp = submenu->addAction(QIcon::fromTheme("cloud-upload"), formatSpeedHumanReadable(statsUp->average()));

            if (mTailscale->isOperator()) {
                submenu->addSection("Taildrop Send");
                submenu->addAction(QIcon::fromTheme(QStringLiteral("document-send")), "Send file(s)", [peer]() {
                    TaildropSendJob::selectAndSendFiles(peer->dnsName());
                });

                // if (peer->isCurrentExitNode()) {
                //     submenu->addAction(QIcon::fromTheme(QStringLiteral("internet-services")), "Unset exit node", []() {
                //         unsetExitNode();
                //     });
                // } else if (peer->isExitNode()) {
                //     submenu->addAction(QIcon::fromTheme(QStringLiteral("internet-services")), "Set as exit node", [peer]() {
                //         setExitNode(peer->tailscaleIps().front());
                //     });
                // }

                if (peer->isRunningSSH()) {
                    submenu->addSection("SSH");
                    submenu->addAction(QIcon::fromTheme(QStringLiteral("akonadiconsole")), "Copy SSH command", [peer]() {
                        setClipboardText(peer->sshCommand());
                    });
                }
            }

            // QObject::connect(statsUp, &SpeedStatistics::refreshed, [actionUp, statsUp]() {
            //     actionUp->setText(formatSpeedHumanReadable(statsUp->average()));
            // });
            // QObject::connect(statsDown, &SpeedStatistics::refreshed, [actionDown, statsDown]() {
            //     actionDown->setText(formatSpeedHumanReadable(statsDown->average()));
            // });
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
    QString icon_path = ":/icons/";
    if (mTailscale->backendState() == "Running") {
        icon_path += "online";
    } else {
        icon_path += "offline";
    }

    const QString style = KTailctlConfig::trayIconStyle();
    if (style == QStringLiteral("Breeze Dark")) {
        icon_path += "-breeze-dark";
    } else if (style == QStringLiteral("Breeze Light")) {
        icon_path += "-breeze-light";
    } else {
        icon_path += "-colorful";
    }
    setIcon(QIcon(icon_path));
}

void TrayIcon::setWindow(QQuickWindow *window)
{
    mWindow = window;
}
