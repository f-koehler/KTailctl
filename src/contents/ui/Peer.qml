// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0
import org.kde.kirigamiaddons.formcard 1.0 as FormCard
import org.kde.kirigamiaddons.labs.components 1.0 as Components
import org.fkoehler.KTailctl.Components 1.0 as MyComponents

Kirigami.ScrollablePage {
    id: peer
    objectName: "Peer"
    title: "Peer: " + App.peerDetails.hostName
    
    header: ColumnLayout {
        Components.Banner {
            width: parent.width
            visible: !Tailscale.status.success
            type: Kirigami.MessageType.Error
            text: i18n("Tailscaled is not running")
        }
        Components.Banner {
            width: parent.width
            visible: (!Tailscale.status.isOperator) && Tailscale.status.success
            type: Kirigami.MessageType.Warning
            text: "KTailctl functionality limited, current user is not the Tailscale operator"
        }
    }

    ColumnLayout {
        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            
            title: i18nc("@title:group", "Information")
        }
        
        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0
                
                MyComponents.FormCopyLabelDelegate {
                    text: i18nc("@label", "Hostname:")
                    copyData: App.peerDetails.hostName
                }
        
                FormCard.FormDelegateSeparator {}
                
                MyComponents.FormCopyLabelDelegate {
                    text: i18nc("@label", "DNS name:")
                    copyData: App.peerDetails.dnsName
                }

                FormCard.FormDelegateSeparator {}

                MyComponents.FormCopyLabelDelegate {
                    text: i18nc("@label", "Tailscale ID:")
                    copyData: App.peerDetails.tailscaleID
                }

                FormCard.FormDelegateSeparator {}

                MyComponents.FormCopyLabelDelegate {
                    text: i18nc("@label", "Created:")
                    copyData: {
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

                FormCard.FormDelegateSeparator {}

                MyComponents.FormCopyLabelDelegate {
                    text: i18nc("@label", "Last seen:")
                    copyData: {
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

                FormCard.FormDelegateSeparator { visible: App.peerDetails.os != "" }

                MyComponents.FormCopyLabelDelegate {
                    text: i18nc("@label", "OS:")
                    copyData: App.peerDetails.os
                    visible: App.peerDetails.os != ""
                }

                FormCard.FormDelegateSeparator {}
                
                MyComponents.FormCopyChipsDelegate {
                    text: i18nc("@label", "Addresses:")
                    model: App.peerDetails.tailscaleIps
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            
            title: i18nc("@title:group", "Exit node")
        }
        
        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0
                
                MyComponents.FormLabeledIconDelegate {
                    text: i18nc("@label", "Exit node:")
                    label: App.peerDetails.isExitNode ? "Yes" : "No"
                    source: App.peerDetails.isExitNode ? "dialog-ok" : "dialog-cancel"
                }

                FormCard.FormDelegateSeparator {}
                
                FormCard.FormSwitchDelegate {
                    text: i18nc("@label", "Use this exit node:")
                    checked: App.peerDetails.isCurrentExitNode
                    enabled: App.peerDetails.isExitNode && !Tailscale.preferences.advertiseExitNode
                    onToggled: {
                        if(App.peerDetails.isCurrentExitNode) {
                            Util.unsetExitNode();
                        } else {
                            Util.setExitNode(App.peerDetails.tailscaleIps[0]);
                        }
                    }
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            
            title: i18nc("@title:group", "Tailscale SSH")
        }
        
        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0
                
                MyComponents.FormLabeledIconDelegate {
                    text: i18nc("@label", "Runs Tailscale SSH:")
                    label: App.peerDetails.isRunningSSH ? "Yes" : "No"
                    source: App.peerDetails.isRunningSSH ? "dialog-ok" : "dialog-cancel"
                }

                FormCard.FormDelegateSeparator {}
                
                MyComponents.FormCopyLabelDelegate {
                    text: i18nc("@label", "SSH command:")
                    copyData: i18nc("@label", "Copy")
                    enabled: App.peerDetails.isRunningSSH
                    onClicked: {
                        Util.Util.setClipboardText(App.peerDetails.sshCommand())
                    }
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            visible: App.peerDetails.location != null
            
            title: i18nc("@title:group", "Location")
        }
        
        FormCard.FormCard {
            Layout.fillWidth: true
            visible: App.peerDetails.location != null

            ColumnLayout {
                spacing: 0
                
                MyComponents.FormLabeledIconDelegate {
                    text: i18nc("@label", "Country:")
                    label: App.peerDetails.location.country + " (" + App.peerDetails.location.countryCode + ")"
                    source: "qrc:/country-flags/" + App.peerDetails.location.countryCode.toLowerCase() + ".svg"
                }

                FormCard.FormDelegateSeparator {}
                
                MyComponents.FormLabelDelegate {
                    text: i18nc("@label", "City:")
                    label: App.peerDetails.location.city + " (" + App.peerDetails.location.cityCode + ")"
                }
            }
        }
    }

    //     Kirigami.Separator {
    //         Kirigami.FormData.isSection: true
    //         Kirigami.FormData.label: "Statistics"
    //     }

    //     Kirigami.Chip {
    //         Kirigami.FormData.label: "Bytes received:"
    //         text: Util..Util..formatCapacityHumanReadable(App.peerDetails.rxBytes)
    //         icon.name: "edit-copy"
    //         closable: false
    //         checkable: false
    //         checked: false
    //         onClicked: {
    //             Util..Util..setClipboardText(App.peerDetails.rxBytes.toString());
    //         }
    //     }

    //     Kirigami.Chip {
    //         Kirigami.FormData.label: "Bytes sent:"
    //         icon.name: "edit-copy"
    //         text: Util..Util..formatCapacityHumanReadable(App.peerDetails.txBytes)
    //         closable: false
    //         checkable: false
    //         checked: false
    //         onClicked: {
    //             Util..Util..setClipboardText(App.peerDetails.txBytes.toString());
    //         }
    //     }

    //     Controls.Label {
    //         Kirigami.FormData.label: "Upload speed:"
    //         text: Util..Util..formatSpeedHumanReadable(Tailscale.statistics.speedUp(App.peerDetails.tailscaleID).average1Second)
    //     }


    //     Controls.Label {
    //         Kirigami.FormData.label: "Download speed:"
    //         text: Util..Util..formatSpeedHumanReadable(Tailscale.statistics.speedDown(App.peerDetails.tailscaleID).average1Second)
    //     }
    // }

    // DropArea {
    //     anchors.fill: parent
    //     onEntered: {
    //         drag.accept (Qt.LinkAction)
    //     }
    //     onDropped: {
    //         TaildropSender.sendFiles(App.peerDetails.dnsName, Util..Util..fileUrlsToStrings(drop.urls))
    //     }
    // }
}
