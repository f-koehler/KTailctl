// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.fkoehler.KTailctl
import org.fkoehler.KTailctl.Components as KTailctlComponents
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: peers

    Layout.fillWidth: true
    objectName: "Peers"
    title: i18n("Peers")

    actions: [
        Kirigami.Action {
            icon.name: "search"
            text: "DNS Regex"

            displayComponent: Kirigami.SearchField {
                id: peerFilter

                text: App.config.peerFilter

                onAccepted: {
                    App.peerModel.setFilterRegularExpression(peerFilter.text);
                    App.config.peerFilter = peerFilter.text;
                    App.config.save();
                }
            }
        }
    ]
    header: KTailctlComponents.HeaderBanner {
    }

    Kirigami.CardsListView {
        id: listPeers

        headerPositioning: ListView.OverlayHeader
        model: App.peerModel
        visible: Tailscale.success

        delegate: Kirigami.AbstractCard {
            contentItem: RowLayout {
                Kirigami.Icon {
                    source: Util.loadOsIcon(os)
                }

                Kirigami.Icon {
                    source: isOnline ? "online" : "offline"
                }

                Controls.ToolButton {
                    icon.name: "edit-copy"
                    text: dnsName

                    onClicked: {
                        Util.setClipboardText(dnsName);
                    }
                }

                Controls.ToolButton {
                    icon.name: "edit-copy"
                    text: tailscaleIps[0]

                    onClicked: {
                        Util.setClipboardText(tailscaleIps[0]);
                    }
                }

                Controls.ToolButton {
                    Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                    Controls.ToolTip.text: text
                    Controls.ToolTip.visible: hovered
                    icon.name: "view-list-details"
                    text: i18nc("@label", "Details")

                    onClicked: {
                        App.setPeerDetails(tailscaleID);
                        pageStack.layers.push(Qt.createComponent("org.fkoehler.KTailctl", "Peer"));
                    }
                }

                Controls.Button {
                    Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                    Controls.ToolTip.text: text
                    Controls.ToolTip.visible: hovered
                    display: Controls.Button.IconOnly
                    icon.name: "menu_new"
                    text: i18nc("@label", "Menu")

                    onClicked: {
                        menu.open();
                    }

                    Controls.Menu {
                        id: menu

                        Controls.MenuItem {
                            icon.name: "akonadiconsole"
                            text: i18nc("@label", "Copy SSH command")
                            visible: isRunningSSH

                            onClicked: {
                                Util.setClipboardText(sshCommand);
                            }
                        }

                        Controls.MenuItem {
                            icon.name: "document-send"
                            text: i18nc("@label", "Send file(s)")

                            onClicked: {
                                TaildropSendJobFactory.selectAndSendFiles(dnsName);
                            }
                        }

                        Controls.MenuItem {
                            icon.name: "internet-services"
                            text: i18nc("@label", isCurrentExitNode ? "Unset exit node" : "Use exit node")
                            visible: isExitNode && !Preferences.advertiseExitNode

                            onClicked: {
                                if (isCurrentExitNode)
                                    Tailscale.unsetExitNode();
                                else
                                    Tailscale.setExitNode(dnsName);
                            }
                        }
                    }
                }
            }

            // DropArea {
            //     anchors.fill: parent

            //     onDropped: {
            //         TaildropSendJobFactory.sendFiles(dnsName, drop.urls);
            //     }
            //     onEntered: {
            //         drag.accept(Qt.LinkAction);
            //     }
            // }
        }
    }
}
