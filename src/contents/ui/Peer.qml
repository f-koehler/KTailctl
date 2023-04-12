// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0

Kirigami.ScrollablePage {
    id: peer

    Layout.fillWidth: true

    Kirigami.FormLayout {
        anchors.fill: parent

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Information"
        }

        Kirigami.Chip {
            Kirigami.FormData.label: "Hostname:"
            text: App.peerDetails.hostName
            closable: false
            checkable: false
            checked: false
            onClicked: {
                Util.setClipboardText(App.peerDetails.hostName);
            }
        }


        Kirigami.Chip {
            Kirigami.FormData.label: "DNS Name:"
            text: App.peerDetails.dnsName
            closable: false
            checkable: false
            checked: false
            onClicked: {
                Util.setClipboardText(App.peerDetails.dnsName);
            }
        }

        Kirigami.Chip {
            Kirigami.FormData.label: "ID:"
            text: App.peerDetails.tailscaleID
            closable: false
            checkable: false
            checked: false
            onClicked: {
                Util.setClipboardText(App.peerDetails.tailscaleID);
            }
        }

        Kirigami.Chip {
            Kirigami.FormData.label: "Created:"
            closable: false
            checkable: false
            checked: false
            text: {
                var duration = Util.formatDurationHumanReadable(App.peerDetails.created);
                if(duration == "") {
                    return "now";
                } else {
                    return duration + " ago";
                }
            }
            onClicked: {
                Util.setClipboardText(Util.toMSecsSinceEpoch(App.peerDetails.created));
            }
        }

        Kirigami.Chip {
            Kirigami.FormData.label: "Last seen:"
            closable: false
            checkable: false
            checked: false
            text: {
                var duration = Util.formatDurationHumanReadable(App.peerDetails.lastSeen);
                if(duration == "") {
                    return "now";
                } else {
                    return duration + " ago";
                }
            }
            onClicked: {
                Util.setClipboardText(Util.toMSecsSinceEpoch(App.peerDetails.lastSeen));
            }
        }


        Flow {
            Kirigami.FormData.label: "OS:"

            Kirigami.Chip {
                text: App.peerDetails.os
                closable: false
                checkable: false
                checked: false
                onClicked: {
                    Util.setClipboardText(App.peerDetails.os);
                }
            }
        }

        Flow {
            Kirigami.FormData.label: "Addresses:"
            Repeater {
                model: App.peerDetails.tailscaleIps

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

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Statistics"
        }

        Kirigami.Chip {
            Kirigami.FormData.label: "Bytes received:"
            text: Util.formatCapacityHumanReadable(App.peerDetails.rxBytes)
            closable: false
            checkable: false
            checked: false
            onClicked: {
                Util.setClipboardText(App.peerDetails.rxBytes.toString());
            }
        }

        Kirigami.Chip {
            Kirigami.FormData.label: "Bytes sent:"
            text: Util.formatCapacityHumanReadable(App.peerDetails.txBytes)
            closable: false
            checkable: false
            checked: false
            onClicked: {
                Util.setClipboardText(App.peerDetails.txBytes.toString());
            }
        }

        Controls.Label {
            Kirigami.FormData.label: "Upload speed:"
            text: Util.formatSpeedHumanReadable(Tailscale.statistics.speedUp(App.peerDetails.tailscaleID).average1Second)
        }


        Controls.Label {
            Kirigami.FormData.label: "Download speed:"
            text: Util.formatSpeedHumanReadable(Tailscale.statistics.speedDown(App.peerDetails.tailscaleID).average1Second)
        }
    }
}
