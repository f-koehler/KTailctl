// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0
import org.kde.kirigami 2.19 as Kirigami

Kirigami.ScrollablePage {
    id: peers

    objectName: "Peers"
    Layout.fillWidth: true
    title: i18n("Peers")
    actions: [
        Kirigami.Action {
            text: "DNS Regex"
            icon.name: "search"

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

    Kirigami.CardsListView {
        id: listPeers

        model: App.peerModel
        visible: Tailscale.success
        headerPositioning: ListView.OverlayHeader

        delegate: Kirigami.AbstractCard {

            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight

                GridLayout {
                    id: delegateLayout

                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.smallSpacing
                    columns: 3

                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                    }

                    Kirigami.Icon {
                        source: Util.loadOsIcon(os)
                    }

                    ColumnLayout {
                        RowLayout {
                            Kirigami.Chip {
                                text: dnsName
                                icon.name: "edit-copy"
                                closable: false
                                checkable: false
                                checked: false
                                onClicked: {
                                    Util.setClipboardText(dnsName);
                                }
                            }

                            Kirigami.Icon {
                                source: isOnline ? "online" : "offline"
                            }

                            // Kirigami.Icon {
                            //     source: "cloud-upload"
                            // }

                            // Controls.Label {
                            //     text: Util.formatSpeedHumanReadable(Tailscale.statistics.speedUp(tailscaleID).average1Second)
                            // }

                            // Kirigami.Icon {
                            //     source: "cloud-download"
                            // }

                            // Controls.Label {
                            //     text: Util.formatSpeedHumanReadable(Tailscale.statistics.speedDown(tailscaleID).average1Second)
                            // }

                        }

                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }

                        Flow {
                            spacing: Kirigami.Units.smallSpacing

                            Repeater {
                                model: tailscaleIps

                                Kirigami.Chip {
                                    text: modelData
                                    icon.name: "edit-copy"
                                    closable: false
                                    checkable: false
                                    checked: false
                                    onClicked: {
                                        Util.setClipboardText(modelData);
                                    }
                                }

                            }

                        }

                    }

                    ColumnLayout {
                        Controls.Button {
                            text: i18nc("@label", "Menu")
                            icon.name: "menu_new"
                            onClicked: {
                                menu.open();
                            }
                            display: Controls.Button.IconOnly
                            Controls.ToolTip.text: text
                            Controls.ToolTip.visible: hovered
                            Controls.ToolTip.delay: Kirigami.Units.toolTipDelay

                            Controls.Menu {
                                id: menu

                                Controls.MenuItem {
                                    text: i18nc("@label", "Copy SSH command")
                                    icon.name: "akonadiconsole"
                                    visible: isRunningSSH
                                    onClicked: {
                                        Util.setClipboardText(sshCommand);
                                    }
                                }

                                Controls.MenuItem {
                                    text: i18nc("@label", "Send file(s)")
                                    icon.name: "document-send"
                                    onClicked: {
                                        TaildropSender.selectAndSendFiles(dnsName);
                                    }
                                }

                                Controls.MenuItem {
                                    text: i18nc("@label", isCurrentExitNode ? "Unset exit node" : "Use exit node")
                                    icon.name: "internet-services"
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

                        Controls.Button {
                            text: i18nc("@label", "Details")
                            icon.name: "view-list-details"
                            onClicked: {
                                App.setPeerDetails(tailscaleID);
                                pageStack.layers.push('qrc:Peer.qml');
                            }
                            display: Controls.Button.IconOnly
                            Controls.ToolTip.text: text
                            Controls.ToolTip.visible: hovered
                            Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                        }

                    }

                }

                DropArea {
                    anchors.fill: parent
                    onEntered: {
                        drag.accept(Qt.LinkAction);
                    }
                    onDropped: {
                        TaildropSender.sendFiles(dnsName, Util.fileUrlsToStrings(drop.urls));
                    }
                }

            }

        }

    }

}
