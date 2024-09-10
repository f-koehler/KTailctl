// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0
import org.kde.kirigami 2.19 as Kirigami

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

    Kirigami.CardsListView {
        id: listPeers

        headerPositioning: ListView.OverlayHeader
        model: App.peerModel
        visible: Tailscale.success

        delegate: Kirigami.AbstractCard {
            contentItem: Item {
                implicitHeight: delegateLayout.implicitHeight
                implicitWidth: delegateLayout.implicitWidth

                GridLayout {
                    id: delegateLayout

                    columnSpacing: Kirigami.Units.smallSpacing
                    columns: 3
                    rowSpacing: Kirigami.Units.largeSpacing

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
                                checkable: false
                                checked: false
                                closable: false
                                icon.name: "edit-copy"
                                text: dnsName

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
                                    checkable: false
                                    checked: false
                                    closable: false
                                    icon.name: "edit-copy"
                                    text: modelData

                                    onClicked: {
                                        Util.setClipboardText(modelData);
                                    }
                                }
                            }
                        }
                    }

                    ColumnLayout {
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
                                        TaildropSendJobs.selectAndSendFiles(dnsName);
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

                        Controls.Button {
                            Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                            Controls.ToolTip.text: text
                            Controls.ToolTip.visible: hovered
                            display: Controls.Button.IconOnly
                            icon.name: "view-list-details"
                            text: i18nc("@label", "Details")

                            onClicked: {
                                App.setPeerDetails(tailscaleID);
                                pageStack.layers.push('qrc:Peer.qml');
                            }
                        }
                    }
                }

                DropArea {
                    anchors.fill: parent

                    onDropped: {
                        TaildropSendJobs.sendFiles(dnsName, Util.fileUrlsToStrings(drop.urls));
                    }
                    onEntered: {
                        drag.accept(Qt.LinkAction);
                    }
                }
            }
        }
    }
}
