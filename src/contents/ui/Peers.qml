// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0

Kirigami.ScrollablePage {
    id: peers

    Layout.fillWidth: true

    title: i18n("Peers")

    actions.main: Kirigami.Action {
        text: Tailscale.status.backendState == "Running" ? "Online" : "Offline"
    }

    Kirigami.CardsListView {
        id: listPeers
        model: App.peerModel
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


                    Controls.Button {
                        text: "Details"
                        icon.name: "view-list-details"
                        onClicked: {
                            App.setPeerDetails(tailscaleID);
                            pageStack.layers.replace('qrc:Peer.qml');
                        }
                    }

                }
            }
        }
    }
}
