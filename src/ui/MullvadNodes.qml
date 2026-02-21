// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Fabian Köhler <me@fkoehler.org>
import QtQuick.Layouts
import QtQuick
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl
import org.fkoehler.KTailctl.Components as KTailctlComponents

Kirigami.ScrollablePage {
    id: mullvadNodes

    Layout.fillWidth: true
    objectName: "MullvadNodes"
    title: i18n("Mullvad Nodes")

    header: KTailctlComponents.HeaderBanner {}

    ColumnLayout {
        FormCard.FormCard {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing

            ColumnLayout {
                spacing: 0

                Repeater {
                    model: App.mullvadNodesForCountryModel

                    delegate: ColumnLayout {
                        FormCard.FormButtonDelegate {
                            icon.name: "country-flag-" + countryCode.toLowerCase()
                            text: dnsName + " (" + city + ")"

                            onClicked: {
                                Tailscale.setExitNode(dnsName);
                            }
                        }

                        FormCard.FormDelegateSeparator {
                            visible: index < App.mullvadNodesForCountryModel.rowCount() - 1
                        }
                    }
                }
            }
        }
    }
}
