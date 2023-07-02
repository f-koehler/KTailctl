// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0

Kirigami.ScrollablePage {
    id: peers
    objectName: "Peers"

    Layout.fillWidth: true

    title: i18n("Peers")

    // actions.main: Kirigami.Action {
    //     text: Tailscale.status.backendState == "Running" ? "Stop tailscale" : "Start tailscale"
    //     onTriggered: App.tailscale.toggle()
    //     icon.name: Tailscale.status.backendState == "Running" ? "process-stop" : "media-playback-start"
    //     visible: Tailscale.status.isOperator
    // }
    actions.main: Kirigami.Action {
        text: {
            if(!Tailscale.status.success) {
                return "Tailscaled is not running"
            } else if(!Tailscale.status.isOperator) {
                return "Functionality limited, current user is not Tailscale operator";
            } else {
                return (Tailscale.status.backendState == "Running") ? "Stop tailscale" : "Start tailscale";
            }
        }
        onTriggered: {
            if(Tailscale.status.isOperator && Tailscale.status.success) {
                App.tailscale.toggle()
            }
        }
        icon.name: {
            if(!Tailscale.status.success) {
                return "emblem-error"
            } else if(!Tailscale.status.isOperator) {
                return "emblem-warning";
            } else {
                return (Tailscale.status.backendState == "Running") ? "process-stop" : "media-playback-start";
            }
        }
    }

    Kirigami.CardsListView {
        id: listPeers
        model: App.peerModel
        visible: Tailscale.status.success
        delegate: Kirigami.AbstractCard {
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout

                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                    }

                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.smallSpacing
                    columns: 3

                    Kirigami.Icon {
                        source: Util.loadOsIcon(os)
                    }

                    ColumnLayout {
                        RowLayout {
                            Controls.Label {
                                text: hostName
                            }

                            Kirigami.Icon {
                                source: isOnline ? "online" : "offline"
                            }

                            Kirigami.Icon {
                                source: "vcs-push"
                            }

                            Controls.Label {
                                text: Util.formatSpeedHumanReadable(Tailscale.statistics.speedUp(tailscaleID).average1Second)
                            }

                            Kirigami.Icon {
                                source: "vcs-pull"
                            }

                            Controls.Label {
                                text: Util.formatSpeedHumanReadable(Tailscale.statistics.speedDown(tailscaleID).average1Second)
                            }
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
                            text: "Details"
                            icon.name: "view-list-details"
                            onClicked: {
                                App.setPeerDetails(tailscaleID);
                                pageStack.layers.push('qrc:Peer.qml');
                            }
                        }
                        Controls.Button {
                            text: "Send file(s)"
                            icon.name: "document-send"
                            onClicked: {
                                TaildropSender.selectAndSendFiles(dnsName)
                            }
                            visible: Tailscale.status.isOperator
                        }
                    }

                }
            }
        }
    }
}
