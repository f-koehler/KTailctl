// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Fabian Köhler <me@fkoehler.org>
import QtQuick.Layouts 1.15
import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard
import org.fkoehler.KTailctl 1.0
import org.fkoehler.KTailctl.Components 1.0 as KTailctlComponents

Kirigami.ScrollablePage {
    id: mullvadNodes

    Layout.fillWidth: true
    objectName: "MullvadNodes"
    title: i18n("Mullvad Nodes")

    header: KTailctlComponents.HeaderBanner {
    }

    ColumnLayout {
        FormCard.FormCard {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing

            ColumnLayout {
                spacing: 0

                Repeater {
                    model: App.mullvadNodesForCountryModel

                    delegate: FormCard.FormButtonDelegate {
                        icon.name: "country-flag-" + countryCode.toLowerCase()
                        text: dnsName + " (" + city + ")"

                        onClicked: {
                            Tailscale.setExitNode(dnsName);
                        }
                    }
                }
            }
        }
    }
}
