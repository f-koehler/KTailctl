// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0
import org.fkoehler.KTailctl.Components 1.0 as KTailctlComponents
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard
import org.kde.kirigamiaddons.labs.components 1.0 as Components

Kirigami.ScrollablePage {
    id: root

    property bool isSelf: false
    property var peer: null

    objectName: "Peer"
    title: "Peer: " + peer.hostName

    header: KTailctlComponents.HeaderBanner {
    }

    DropArea {
        anchors.fill: parent

        onDropped: {
            TaildropSendJobFactory.sendFiles(peer.dnsName, drop.urls);
        }
        onEntered: {
            drag.accept(Qt.LinkAction);
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

                KTailctlComponents.FormCopyLabelDelegate {
                    copyData: root.peer.hostName
                    text: i18nc("@label", "Hostname:")
                }

                FormCard.FormDelegateSeparator {
                }

                KTailctlComponents.FormCopyLabelDelegate {
                    copyData: root.peer.dnsName
                    text: i18nc("@label", "DNS name:")
                }

                FormCard.FormDelegateSeparator {
                }

                KTailctlComponents.FormCopyLabelDelegate {
                    copyData: root.peer.tailscaleID
                    text: i18nc("@label", "Tailscale ID:")
                }

                FormCard.FormDelegateSeparator {
                }

                KTailctlComponents.FormCopyLabelDelegate {
                    copyData: {
                        if (!root.peer.created) {
                            return "undefined";
                        }
                        var duration = Util.formatDurationHumanReadable(peer.created);
                        if (duration == "")
                            return "now";
                        else
                            return duration + " ago";
                    }
                    text: i18nc("@label", "Created:")

                    onClicked: {
                        Util.setClipboardText(Util.toMSecsSinceEpoch(peer.created));
                    }
                }

                FormCard.FormDelegateSeparator {
                    visible: !root.isSelf
                }

                KTailctlComponents.FormCopyLabelDelegate {
                    copyData: {
                        if (!root.peer.lastSeen) {
                            return "undefined";
                        }
                        var duration = Util.formatDurationHumanReadable(peer.lastSeen);
                        if (duration == "")
                            return "now";
                        else
                            return duration + " ago";
                    }
                    text: i18nc("@label", "Last seen:")
                    visible: !root.isSelf

                    onClicked: {
                        Util.setClipboardText(Util.toMSecsSinceEpoch(peer.lastSeen));
                    }
                }

                FormCard.FormDelegateSeparator {
                    visible: root.peer.os != ""
                }

                KTailctlComponents.FormCopyLabelDelegate {
                    copyData: root.peer.os
                    text: i18nc("@label", "OS:")
                    visible: root.peer.os != ""
                }

                FormCard.FormDelegateSeparator {
                }

                KTailctlComponents.FormCopyChipsDelegate {
                    model: root.peer.tailscaleIps
                    text: i18nc("@label", "Addresses:")
                }

                FormCard.FormDelegateSeparator {
                    visible: root.peer.tags.length > 0
                }

                KTailctlComponents.FormCopyChipsDelegate {
                    model: root.peer.tags
                    text: i18nc("@label", "Tags:")
                    visible: root.peer.tags.length > 0
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

                KTailctlComponents.FormLabeledIconDelegate {
                    label: root.peer.isExitNode ? "Yes" : "No"
                    source: root.peer.isExitNode ? "dialog-ok" : "dialog-cancel"
                    text: i18nc("@label", "Exit node:")
                }

                FormCard.FormDelegateSeparator {
                }

                FormCard.FormSwitchDelegate {
                    checked: root.peer.isCurrentExitNode
                    enabled: root.peer.isExitNode && !Preferences.advertiseExitNode
                    text: i18nc("@label", "Use this exit node:")

                    onToggled: {
                        if (root.peer.isCurrentExitNode)
                            Tailscale.unsetExitNode();
                        else
                            Tailscale.setExitNode(peer.dnsName);
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

                KTailctlComponents.FormLabeledIconDelegate {
                    label: root.peer.isRunningSSH() ? "Yes" : "No"
                    source: root.peer.isRunningSSH() ? "dialog-ok" : "dialog-cancel"
                    text: i18nc("@label", "Runs Tailscale SSH:")
                }

                FormCard.FormDelegateSeparator {
                }

                KTailctlComponents.FormCopyLabelDelegate {
                    copyData: i18nc("@label", "Copy")
                    enabled: root.peer.isRunningSSH()
                    text: i18nc("@label", "SSH command:")

                    onClicked: {
                        Util.setClipboardText(peer.sshCommand());
                    }
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Location")
            visible: root.peer.location != null
        }

        FormCard.FormCard {
            Layout.fillWidth: true
            visible: root.peer.location != null

            ColumnLayout {
                spacing: 0

                KTailctlComponents.FormLabeledIconDelegate {
                    label: root.peer.location == null ? "" : peer.location.country + " (" + peer.location.countryCode + ")"
                    source: root.peer.location == null ? "question" : "qrc:/country-flags/" + peer.location.countryCode.toLowerCase() + ".svg"
                    text: i18nc("@label", "Country:")

                    onClicked: {
                        Util.setClipboardText(peer.location.country);
                    }
                }

                FormCard.FormDelegateSeparator {
                }

                KTailctlComponents.FormLabelDelegate {
                    label: root.peer.location == null ? "" : peer.location.city + " (" + peer.location.cityCode + ")"
                    text: i18nc("@label", "City:")

                    onClicked: {
                        Util.setClipboardText(peer.location.city);
                    }
                }
            }
        }

        // FormCard.FormHeader {
        //     Layout.fillWidth: true
        //     Layout.topMargin: Kirigami.Units.largeSpacing
        //     visible: !root.isSelf
        //     title: i18nc("@title:group", "Statistics")
        // }

        // FormCard.FormCard {
        //     Layout.fillWidth: true
        //     visible: !root.isSelf

        //     ColumnLayout {
        //         spacing: 0

        //         KTailctlComponents.FormLabeledIconDelegate {
        //             text: i18nc("@label", "Download:")
        //             label: root.isSelf ? "" : Util.formatCapacityHumanReadable(peer.rxBytes) + " (" + Util.formatSpeedHumanReadable(Tailscale.statistics.speedDown(peer.tailscaleID).average1Second) + ")"
        //             source: "cloud-download"
        //             onClicked: {
        //                 Util.setClipboardText(peer.rxBytes);
        //             }
        //         }

        //         FormCard.FormDelegateSeparator {
        //         }

        //         KTailctlComponents.FormLabeledIconDelegate {
        //             text: i18nc("@label", "Upload:")
        //             label: root.isSelf ? "" : Util.formatCapacityHumanReadable(peer.txBytes) + " (" + Util.formatSpeedHumanReadable(Tailscale.statistics.speedUp(peer.tailscaleID).average1Second) + ")"
        //             source: "cloud-upload"
        //             onClicked: {
        //                 Util.setClipboardText(peer.txBytes);
        //             }
        //         }

        //     }

        // }

    }
}
