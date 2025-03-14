// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick.Layouts
import QtQuick
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl
import org.fkoehler.KTailctl.Components as KTailctlComponents

Kirigami.ScrollablePage {
    id: exitNodes

    Layout.fillWidth: true
    objectName: "ExitNodes"
    title: i18n("Exit Nodes")

    header: KTailctlComponents.HeaderBanner {
    }

    ColumnLayout {
        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "No exit nodes available!")
            visible: (Tailscale.exitNodeModel.rowCount() == 0) && (Tailscale.mullvadNodeModel.rowCount() == 0)
        }

        // FormCard.FormHeader {
        //     Layout.fillWidth: true
        //     Layout.topMargin: Kirigami.Units.largeSpacing
        //     title: i18nc("@title:group", "Current")
        //     visible: (Tailscale.exitNodeModel.rowCount() > 0) || (Tailscale.mullvadNodeModel.rowCount() > 0)
        // }

        FormCard.FormCard {
            Layout.fillWidth: true
            visible: (Tailscale.exitNodeModel.rowCount() > 0) || (Tailscale.mullvadNodeModel.rowCount() > 0)

            ColumnLayout {
                spacing: 0

                KTailctlComponents.FormLabelDelegate {
                    text: i18nc("@label", "Currently not using any exit node.")
                    visible: Tailscale.currentExitNode == null
                }

                FormCard.FormButtonDelegate {
                    icon.name: "dialog-cancel"
                    text: "Unset current: " + Tailscale.currentExitNode.dnsName
                    visible: Tailscale.currentExitNode != null

                    onClicked: {
                        Tailscale.unsetExitNode();
                    }
                }

                FormCard.FormDelegateSeparator {
                    visible: Tailscale.hasSuggestedExitNode
                }

                FormCard.FormButtonDelegate {
                    icon.name: Tailscale.suggestedExitNode.isMullvad ? "country-flag-" + Tailscale.suggestedExitNode.countryCode.toLowerCase() : "network-vpn"
                    text: "Use suggested: " + Tailscale.suggestedExitNode.dnsName
                    visible: Tailscale.hasSuggestedExitNode

                    onClicked: {
                        Tailscale.setExitNode(Tailscale.suggestedExitNode.dnsName);
                    }
                }

                FormCard.FormDelegateSeparator {
                    visible: App.config.lastUsedExitNode.length > 0
                }

                FormCard.FormButtonDelegate {
                    icon.name: "folder-recent-symbolic"
                    text: "Last used: " + App.config.lastUsedExitNode
                    visible: App.config.lastUsedExitNode.length > 0

                    onClicked: {
                        Tailscale.setExitNode(App.config.lastUsedExitNode);
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
                    visible: Tailscale.exitNodeModel.rowCount() == 0
                }

                Repeater {
                    model: Tailscale.exitNodeModel

                    delegate: ColumnLayout {
                        FormCard.FormButtonDelegate {
                            icon.name: "network-vpn"
                            text: dnsName

                            onClicked: {
                                Tailscale.setExitNode(dnsName);
                            }
                        }

                        FormCard.FormDelegateSeparator {
                            visible: index < Tailscale.exitNodeModel.rowCount() - 1
                        }
                    }
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Mullvad")
            visible: Tailscale.mullvadNodeModel.rowCount() > 0
        }

        FormCard.FormCard {
            Layout.fillWidth: true
            visible: Tailscale.mullvadNodeModel.rowCount() > 0

            ColumnLayout {
                spacing: 0

                Repeater {
                    model: Tailscale.mullvadCountryModel

                    delegate: ColumnLayout {
                        FormCard.FormButtonDelegate {
                            icon.name: "country-flag-" + countryCode.toLowerCase()
                            text: countryName

                            onClicked: {
                                App.mullvadNodesForCountryModel.setFilterFixedString(countryCode);
                                pageStack.layers.push(Qt.createComponent("org.fkoehler.KTailctl", "MullvadNodes"));
                            }
                        }

                        FormCard.FormDelegateSeparator {
                            visible: index < Tailscale.mullvadCountryModel.rowCount() - 1
                        }
                    }
                }
            }
        }
    }
}
