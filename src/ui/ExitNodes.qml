// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick.Layouts 1.15
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
                    visible: App.tailscale.status.exitNode == null
                }

                FormCard.FormButtonDelegate {
                    text: "Unset current: " + App.tailscale.status.exitNode.dnsName
                    visible: App.tailscale.status.exitNode != null
                    onClicked: {
                        Util.unsetExitNode()
                    }
                }

            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Available")
        }

        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0
                FormCard.FormButtonDelegate {
                    text: "Use suggested: " + App.tailscale.status.suggestedExitNode.dnsName
                    visible: App.tailscale.status.suggestedExitNode != null
                    onClicked: {
                        Util.setExitNode(App.tailscale.status.suggestedExitNode)
                    }
                }
            }
        }
    }
}
