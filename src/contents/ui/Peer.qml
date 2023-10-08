// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0

Kirigami.ScrollablePage {
    id: peer
    objectName: "Peer"
    title: "Peer: " + App.peerDetails.hostName

    Layout.fillWidth: true

    Kirigami.FormLayout {
        width: parent.width

        Kirigami.InlineMessage {
            Layout.fillWidth: true
            visible: !Tailscale.status.success
            type: Kirigami.MessageType.Error
            text: "Tailscaled is not running"
        }

        Kirigami.InlineMessage {
            Layout.fillWidth: true
            visible: (!Tailscale.status.isOperator) && Tailscale.status.success
            type: Kirigami.MessageType.Warning
            text: "KTailctl functionality limited, current user is not the Tailscale operator"
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Information"
        }

        Kirigami.Chip {
            Kirigami.FormData.label: "Hostname:"
            text: App.peerDetails.hostName
            icon.name: "edit-copy"
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
            icon.name: "edit-copy"
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
            icon.name: "edit-copy"
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
            icon.name: "edit-copy"
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
            icon.name: "edit-copy"
            onClicked: {
                Util.setClipboardText(Util.toMSecsSinceEpoch(App.peerDetails.lastSeen));
            }
        }


        Flow {
            Kirigami.FormData.label: "OS:"

            Kirigami.Chip {
                text: App.peerDetails.os
                icon.name: "edit-copy"
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

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Exit Node"
        }

        Controls.Label {
            Kirigami.FormData.label: "Runs exit node:"
            text: App.peerDetails.isExitNode ? "Yes" : "No"
        }

        Controls.Switch {
            Kirigami.FormData.label: "Set as exit node:"
            checked: App.peerDetails.isCurrentExitNode
            enabled: App.peerDetails.isExitNode
            onToggled: {
                if(App.peerDetails.isCurrentExitNode) {
                    Util.unsetExitNode();
                } else {
                    Util.setExitNode(App.peerDetails.tailscaleIps[0]);
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
            icon.name: "edit-copy"
            closable: false
            checkable: false
            checked: false
            onClicked: {
                Util.setClipboardText(App.peerDetails.rxBytes.toString());
            }
        }

        Kirigami.Chip {
            Kirigami.FormData.label: "Bytes sent:"
            icon.name: "edit-copy"
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

    DropArea {
        anchors.fill: parent
        onEntered: {
            drag.accept (Qt.LinkAction)
        }
        onDropped: {
            TaildropSender.sendFiles(App.peerDetails.dnsName, Util.fileUrlsToStrings(drop.urls))
        }
    }
}
