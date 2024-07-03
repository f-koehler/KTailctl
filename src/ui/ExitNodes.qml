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

    ColumnLayout {
        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Current")
        }

        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0

                KTailctlComponents.FormLabelDelegate {
                    text: i18nc("@label", "Currently not using any exit node.")
                    visible: Tailscale.currentExitNode == null
                }

                FormCard.FormButtonDelegate {
                    text: "Unset current: " + Tailscale.currentExitNode.dnsName
                    visible: Tailscale.currentExitNode != null
                    onClicked: {
                        Tailscale.unsetExitNode();
                    }
                }

                FormCard.FormDelegateSeparator {
                }

                FormCard.FormButtonDelegate {
                    text: "Use suggested: " + Tailscale.suggestedExitNode.dnsName
                    visible: Tailscale.suggestedExitNode != null
                    onClicked: {
                        Tailscale.setExitNode(Tailscale.suggestedExitNode);
                    }
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Self-hosted")
        }

        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0

                FormCard.FormTextDelegate {
                    text: "None available."
                    visible: Tailscale.exitNodes.length == 0
                }

                Repeater {
                    model: Tailscale.exitNodeModel
                    delegate: FormCard.FormButtonDelegate {
                        text: dnsName
                        onClicked: {
                            Tailscale.setExitNodeFromDnsName(dnsName);
                        }
                    }
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Mullvad")
        }

        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0
                Repeater {
                    model: Tailscale.mullvadCountryModel
                    delegate: FormCard.FormButtonDelegate {
                        text: countryName
                        onClicked: {
                            App.mullvadNodesForCountryModel.setFilterFixedString(countryCode)
                            pageStack.layers.push("qrc:MullvadNodes.qml");
                        }
                    }
                }
            }
        }
    }
}
