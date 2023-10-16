// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tray_icon.h"
#include "util.h"

#include <QClipboard>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenu>
#include <QMessageBox>

TrayIcon::TrayIcon(Tailscale *tailscale, QObject *parent)
    : QSystemTrayIcon(parent)
    , mTailscale(tailscale)
{
    if (mTailscale->status()->backendState() == "Running") {
        setIcon(QIcon(QStringLiteral(":/icons/online")));
    } else {
        setIcon(QIcon(QStringLiteral(":/icons/offline")));
    }

    setContextMenu(new QMenu());

    QObject::connect(contextMenu(), &QMenu::aboutToShow, this, &TrayIcon::regenerate);
    QObject::connect(this, &QSystemTrayIcon::activated, [this](const QSystemTrayIcon::ActivationReason &) {
        if (mWindow == nullptr) {
            return;
        }
        if (mWindow->isVisible()) {
            mWindow->hide();
        } else {
            mWindow->show();
        }
    });

    show();
}

void TrayIcon::regenerate()
{
    QMenu *menu = contextMenu();
    menu->clear();

    if (mTailscale->status()->backendState() == "Running") {
        setIcon(QIcon(QStringLiteral(":/icons/online")));
    } else {
        setIcon(QIcon(QStringLiteral(":/icons/offline")));
    }

    if (mWindow != nullptr) {
        menu->addAction(QIcon::fromTheme("window"), "Open", this, [this]() {
            mWindow->show();
        });
        menu->addSeparator();
    }

    if (mTailscale->status()->isOperator() && mTailscale->status()->success()) {
        auto *action_toggle = menu->addAction("Toggle", [this]() {
            mTailscale->toggle();
        });
        if (mTailscale->status()->backendState() == "Running") {
            action_toggle->setChecked(true);
            action_toggle->setText("Stop Tailscale");
            action_toggle->setIcon(QIcon::fromTheme(QStringLiteral("process-stop")));
        } else {
            action_toggle->setChecked(false);
            action_toggle->setText("Start Tailscale");
            action_toggle->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
        }

        const auto [exit_nodes, mullvad_nodes] = mTailscale->status()->exitNodes();
        if ((exit_nodes.size() > 0) || (mullvad_nodes.size())) {
            auto *menu_exit_nodes = menu->addMenu(QIcon::fromTheme("internet-services"), "Exit Nodes");

            if (exit_nodes.size() > 0) {
                auto *menu_self_hosted = menu_exit_nodes->addMenu(QIcon::fromTheme("internet-services"), "Self-Hosted");
                for (const auto *node : exit_nodes) {
                    menu_self_hosted->addAction(Util::loadOsIcon(node->os()), node->hostName(), [node]() {
                        Util::setExitNode(node->tailscaleIps().front());
                    });
                }
            }

            if (mullvad_nodes.size() > 0) {
                auto *menu_mullvad_nodes = menu_exit_nodes->addMenu(QIcon::fromTheme("internet-services"), "Mullvad Exit Nodes");
                QMap<QString, QMenu *> mullvad_menus;
                for (const auto *node : mullvad_nodes) {
                    if (node->location() == nullptr) {
                        continue;
                    }
                    const auto country_code = node->location()->countryCode();
                    auto menu_pos = mullvad_menus.lowerBound(country_code);
                    if (menu_pos.key() != country_code) {
                        menu_pos = mullvad_menus.insert(country_code, menu_mullvad_nodes->addMenu(QIcon::fromTheme("internet-services"), country_code));
                    }
                    menu_pos.value()->addAction(QIcon::fromTheme(QStringLiteral("internet-services")), node->hostName(), [node]() {
                        Util::setExitNode(node->tailscaleIps().front());
                    });
                }
            }
        }

        // const auto &exit_nodes = mTailscale->status()->exitNodes();
        // if (exit_nodes.size() > 0) {
        //     auto *menu_exit_node = menu->addMenu(QIcon::fromTheme("internet-services"), "Exit Node");
        //     for (const auto &exit_node : exit_nodes) {
        //         menu_exit_node->addAction(QIcon::fromTheme(QStringLiteral("internet-services")), exit_node, [&exit_node]() {});
        //     }
        // }
        menu->addSeparator();
    }

    auto create_action = [](QMenu *menu, const QString &text) {
        auto *action = menu->addAction(QIcon::fromTheme("edit-copy"), text, [text]() {
            setClipboardText(text);
        });
        return action;
    };

    if (mTailscale->status()->success()) {
        auto *peer_menu = menu->addMenu(QIcon::fromTheme("applications-network"), "Peers");
        for (auto *peer : mTailscale->status()->peers()) {
            if (peer->isMullvad()) {
                continue;
            }
            auto *submenu = peer_menu->addMenu(loadOsIcon(peer->os()), peer->hostName());
            create_action(submenu, peer->dnsName());
            for (const auto &address : peer->tailscaleIps()) {
                create_action(submenu, address);
            }
            submenu->addSection("Statistics");

            auto *statsUp = mTailscale->statistics()->speedUp(peer->id());
            auto *statsDown = mTailscale->statistics()->speedDown(peer->id());
            auto *actionUp = submenu->addAction(QIcon::fromTheme("vcs-push"), formatSpeedHumanReadable(statsUp->average()));
            auto *actionDown = submenu->addAction(QIcon::fromTheme("vcs-pull"), formatSpeedHumanReadable(statsDown->average()));

            if (mTailscale->status()->isOperator()) {
                submenu->addSection("Taildrop Send");
                submenu->addAction(QIcon::fromTheme(QStringLiteral("document-send")), "Send file(s)", [peer]() {
                    TaildropSendJob::selectAndSendFiles(peer->dnsName());
                });

                // if (peer->isCurrentExitNode()) {
                //     submenu->addAction(QIcon::fromTheme(QStringLiteral("internet-services")), "Unset exit node", []() {
                //         Util::unsetExitNode();
                //     });
                // } else if (peer->isExitNode()) {
                //     submenu->addAction(QIcon::fromTheme(QStringLiteral("internet-services")), "Set as exit node", [peer]() {
                //         Util::setExitNode(peer->tailscaleIps().front());
                //     });
                // }

                if (peer->isRunningSSH()) {
                    submenu->addSection("SSH");
                    submenu->addAction(QIcon::fromTheme(QStringLiteral("akonadiconsole")), "Copy SSH command", [peer]() {
                        Util::setClipboardText(peer->getSSHCommand());
                    });
                }
            }

            QObject::connect(statsUp, &SpeedStatistics::refreshed, [actionUp, statsUp]() {
                actionUp->setText(formatSpeedHumanReadable(statsUp->average()));
            });
            QObject::connect(statsDown, &SpeedStatistics::refreshed, [actionDown, statsDown]() {
                actionDown->setText(formatSpeedHumanReadable(statsDown->average()));
            });
        }
    }

    menu->addSeparator();
    menu->addAction(QIcon::fromTheme("application-exit"), "Quit", [this]() {
        emit quitClicked();
    });
    setContextMenu(menu);
}

void TrayIcon::setWindow(QQuickWindow *window)
{
    mWindow = window;
}
