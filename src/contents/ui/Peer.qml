// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.Tailctl 1.0

Kirigami.ScrollablePage {
    id: peer

    Layout.fillWidth: true

    Kirigami.FormLayout {
        anchors.fill: parent

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Information"
        }

        Controls.TextField {
            text: PeerDetails.hostName
            readOnly: true
            Kirigami.FormData.label: "Hostname:"
        }


        Controls.TextField {
            text: PeerDetails.dnsName
            readOnly: true
            Kirigami.FormData.label: "DNS name:"
        }

        Controls.TextField {
            text: PeerDetails.tailscaleID
            readOnly: true
            Kirigami.FormData.label: "ID:"
        }

        RowLayout {
            Kirigami.FormData.label: "Operating system:"

            Controls.Label {
                text: PeerDetails.os
            }

            Kirigami.Icon {
                source: getOSIcon(PeerDetails.os)
                isMask: true
            }
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Adresses"
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Statistics"
        }

        Controls.Label {
            Kirigami.FormData.label: "Bytes received:"
            text: PeerDetails.rxBytes.toString()
        }


        Controls.Label {
            Kirigami.FormData.label: "Bytes sent:"
            text: PeerDetails.rxBytes.toString()
        }
    }
}
