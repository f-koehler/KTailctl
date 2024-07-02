// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick.Layouts 1.15
import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard
import org.fkoehler.KTailctl 1.0
import org.fkoehler.KTailctl.Components 1.0 as KTailctlComponents

Kirigami.ScrollablePage {
    id: exitNodes

    objectName: "ExitNodes"
    Layout.fillWidth: true
    title: i18n("Exit Nodes")

    // ColumnLayout {
    // FormCard.FormHeader {
    //     Layout.fillWidth: true
    //     Layout.topMargin: Kirigami.Units.largeSpacing
    //     title: i18nc("@title:group", "Current")
    // }

    // FormCard.FormCard {
    //     Layout.fillWidth: true

    //     ColumnLayout {
    //         spacing: 0

    //         KTailctlComponents.FormLabelDelegate {
    //             text: i18nc("@label", "Currently not using any exit node.")
    //             visible: Tailscale.currentExitNode == null
    //         }

    //         // FormCard.FormButtonDelegate {
    //         //     text: "Unset current: " + Tailscale.currentExitNode.dnsName
    //         //     visible: Tailscale.currentExitNode != null
    //         //     onClicked: {
    //         //         Tailscale.unsetExitNode();
    //         //     }
    //         // }
    //     }
    // }

    // FormCard.FormHeader {
    //     Layout.fillWidth: true
    //     Layout.topMargin: Kirigami.Units.largeSpacing
    //     title: i18nc("@title:group", "Available")
    // }

    // FormCard.FormCard {
    //     Layout.fillWidth: true

    //     ColumnLayout {
    //         spacing: 0
    //         FormCard.FormButtonDelegate {
    //             text: "Use suggested: " + Tailscale.suggestedExitNode.dnsName
    //             visible: Tailscale.suggestedExitNode != null
    //             onClicked: {
    //                 Tailscale.setExitNode(Tailscale.suggestedExitNode);
    //             }
    //         }
    //     }
    // }

    Kirigami.CardsListView {
        model: Tailscale.exitNodeModel

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

                    RowLayout {
                        Kirigami.Icon {
                            source: Util.loadOsIcon(os)
                        }

                        Controls.Label {
                            text: dnsName
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

                        Controls.Button {
                            text: i18nc("@label", "Use")
                            icon.name: "dialog-ok-apply"
                            onClicked: {
                                Tailscale.setExitNodeFromDnsName(dnsName);
                            }
                            display: Controls.Button.IconOnly
                            Controls.ToolTip.text: text
                            Controls.ToolTip.visible: hovered
                            Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                        }
                    }
                }
            }
        }
    }
}
