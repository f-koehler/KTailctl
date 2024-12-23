// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Dialogs
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0
import org.fkoehler.KTailctl.Components 1.0 as KTailctlComponents
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard
import org.kde.kirigamiaddons.labs.components 1.0 as Components

Kirigami.ScrollablePage {
    id: settings

    leftPadding: 0
    objectName: "Settings"
    rightPadding: 0
    title: i18n("Settings")

    actions: [
        Kirigami.Action {
            icon.name: (Tailscale.backendState == "Running") ? "process-stop" : "media-playback-start"
            text: (Tailscale.backendState == "Running") ? "Stop tailscale" : "Start tailscale"
            visible: Tailscale.success && Tailscale.isOperator

            onTriggered: {
                Tailscale.toggle();
            }
        }
    ]
    header: Components.Banner {
        text: i18n("Tailscaled is not running")
        type: Kirigami.MessageType.Error
        visible: !Tailscale.success
        width: parent.width
    }

    ColumnLayout {
        // Controls.ComboBox {
        //     id: comboTaildropStrategy
        //     Kirigami.FormCard.label: "Strategy:"
        //     model: ["Rename", "Overwrite", "Skip"]
        //     onActivated: {
        //         App.config.taildropStrategy = comboTaildropStrategy.currentText;
        //         App.config.save()
        //     }
        //     enabled: Tailscale.isOperator
        // }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Interface")
        }

        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0

                FormCard.FormSwitchDelegate {
                    id: startMinimized

                    checked: App.config.startMinimized
                    enabled: App.config.enableTrayIcon
                    text: i18nc("@label", "Start minimized:")

                    onClicked: {
                        App.config.startMinimized = !App.config.startMinimized;
                        App.config.save();
                    }
                }

                FormCard.FormDelegateSeparator {
                    above: enableTrayIcon
                    below: startMinimized
                }

                FormCard.FormSwitchDelegate {
                    id: enableTrayIcon

                    checked: App.config.enableTrayIcon
                    text: i18nc("@label", "Enable tray icon:")

                    onClicked: {
                        App.config.enableTrayIcon = !App.config.enableTrayIcon;
                        if (!App.config.enableTrayIcon)
                            App.config.startMinimized = false;
                        App.config.save();
                        App.trayIcon.visible = App.config.enableTrayIcon;
                    }
                }

                FormCard.FormDelegateSeparator {
                    above: trayIconTheme
                    below: enableTrayIcon
                }

                FormCard.FormComboBoxDelegate {
                    id: trayIconTheme

                    displayText: App.config.trayIconTheme
                    model: Preferences.trayIconThemes
                    text: i18nc("@label", "Tray icon theme:")

                    onActivated: {
                        App.config.trayIconTheme = trayIconTheme.currentText;
                        App.config.save();
                    }
                }

                FormCard.FormDelegateSeparator {
                    above: spinRefreshInterval
                    below: trayIconTheme
                }

                FormCard.FormSpinBoxDelegate {
                    id: spinRefreshInterval

                    from: 10
                    label: i18nc("@label", "Refresh rate (ms):")
                    stepSize: 100
                    to: 10000
                    value: refreshStatusTimer.interval

                    onValueChanged: {
                        refreshStatusTimer.interval = spinRefreshInterval.value;
                        App.config.refreshInterval = spinRefreshInterval.value;
                        App.config.save();
                    }
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Tailscale")
        }

        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0

                FormCard.FormSwitchDelegate {
                    id: enableTailscale

                    checked: Tailscale.backendState == "Running"
                    enabled: Tailscale.isOperator && Tailscale.success
                    text: i18nc("@label", "Enable Tailscale:")

                    onClicked: {
                        Tailscale.toggle();
                    }
                }

                FormCard.FormDelegateSeparator {
                    above: enableExitNodeLanAccess
                    below: enableTailscale
                }

                FormCard.FormSwitchDelegate {
                    id: enableExitNodeLanAccess

                    checked: Preferences.allowLANAccess
                    enabled: Tailscale.isOperator && Tailscale.success
                    text: i18nc("@label", "Allow LAN Access:")

                    onClicked: Preferences.allowLANAccess = !Preferences.allowLANAccess
                }

                FormCard.FormDelegateSeparator {
                    above: acceptRoutes
                    below: enableExitNodeLanAccess
                }

                FormCard.FormSwitchDelegate {
                    id: acceptRoutes

                    checked: Preferences.acceptRoutes
                    enabled: Tailscale.isOperator && Tailscale.success
                    text: i18nc("@label", "Accept Routes:")

                    onClicked: Preferences.acceptRoutes = !Preferences.acceptRoutes
                }

                FormCard.FormDelegateSeparator {
                    above: acceptDNS
                    below: acceptRoutes
                }

                FormCard.FormSwitchDelegate {
                    id: acceptDNS

                    checked: Preferences.acceptDNS
                    enabled: Tailscale.isOperator && Tailscale.success
                    text: i18nc("@label", "Accept DNS:")

                    onClicked: Preferences.acceptDNS = !Preferences.acceptDNS
                }

                FormCard.FormDelegateSeparator {
                    above: textTailscaleHostname
                    below: acceptDNS
                }

                FormCard.FormTextFieldDelegate {
                    id: textTailscaleHostname

                    enabled: Tailscale.isOperator && Tailscale.success
                    label: i18nc("@label", "Hostname:")
                    text: Preferences.hostname

                    onEditingFinished: {
                        Preferences.hostname = textTailscaleHostname.text;
                    }
                }

                FormCard.FormDelegateSeparator {
                    above: shieldsUp
                    below: textTailscaleHostname
                }

                FormCard.FormSwitchDelegate {
                    id: shieldsUp

                    checked: Preferences.shieldsUp
                    enabled: Tailscale.isOperator && Tailscale.success
                    text: i18nc("@label", "Shields up:")

                    onClicked: Preferences.shieldsUp = !Preferences.shieldsUp
                }

                FormCard.FormDelegateSeparator {
                    above: ssh
                    below: shieldsUp
                }

                FormCard.FormSwitchDelegate {
                    id: ssh

                    checked: Preferences.ssh
                    enabled: Tailscale.isOperator && Tailscale.success
                    text: i18nc("@label", "SSH:")

                    onClicked: Preferences.ssh = !Preferences.ssh
                }

                FormCard.FormDelegateSeparator {
                    above: shieldsUp
                    below: webclient
                }

                FormCard.FormSwitchDelegate {
                    id: webclient

                    checked: Preferences.webClient
                    enabled: Tailscale.isOperator && Tailscale.success
                    text: i18nc("@label", "Web interface:")

                    onClicked: Preferences.webClient = !Preferences.webClient
                }

                FormCard.FormDelegateSeparator {
                    visible: Preferences.webClient && Tailscale.isOperator
                }

                KTailctlComponents.FormCopyLabelDelegate {
                    copyData: Tailscale.self.dnsName + ":5252"
                    text: i18nc("@label", "Web interface URL:")
                    visible: Preferences.webClient && Tailscale.isOperator
                }
            }
        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Exit Node")
        }

        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0

                FormCard.FormSwitchDelegate {
                    id: advertiseExitNode

                    checked: Preferences.advertiseExitNode
                    enabled: Tailscale.isOperator && Tailscale.success
                    text: i18nc("@label", "Run exit node:")

                    onClicked: {
                        if (!Preferences.advertiseExitNode) {
                            Tailscale.unsetExitNode();
                            Preferences.advertiseExitNode = true;
                        } else {
                            Preferences.advertiseExitNode = false;
                        }
                    }
                }
            }
        }

        // FormCard.FormHeader {
        //     Layout.fillWidth: true
        //     Layout.topMargin: Kirigami.Units.largeSpacing
        //     title: i18nc("@title:group", "Taildrop (requires restart)")
        // }

        // FormCard.FormCard {
        //     Layout.fillWidth: true

        //     ColumnLayout {
        //         spacing: 0

        //         FormCard.FormSwitchDelegate {
        //             id: status

        //             checked: App.config.taildropEnabled
        //             enabled: Tailscale.isOperator && Tailscale.success
        //             text: i18nc("@label", "Status:")

        //             onClicked: {
        //                 App.config.taildropEnabled = !App.config.taildropEnabled;
        //                 App.config.save();
        //             }
        //         }

        //         FormCard.FormDelegateSeparator {
        //             above: status
        //         }

        //         FormCard.AbstractFormDelegate {
        //             enabled: Tailscale.isOperator && Tailscale.success

        //             contentItem: ColumnLayout {
        //                 Controls.Label {
        //                     text: i18nc("@label", "Directory")
        //                 }

        //                 RowLayout {
        //                     Controls.TextField {
        //                         id: textTaildropDirectory

        //                         Layout.fillWidth: true
        //                         text: App.config.taildropDirectory

        //                         onEditingFinished: {
        //                             App.config.taildropDirectory = textTaildropDirectory.text;
        //                             App.config.save();
        //                         }
        //                     }

        //                     FolderDialog {
        //                         id: folderDialogTaildropDirectory

        //                         currentFolder: App.config.taildropDirectory

        //                         onAccepted: {
        //                             App.config.taildropDirectory = App.fileUrlToString(folderDialogTaildropDirectory.fileUrls[0]);
        //                             App.config.save();
        //                         }
        //                     }

        //                     Controls.Button {
        //                         Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
        //                         Controls.ToolTip.text: text
        //                         Controls.ToolTip.visible: hovered
        //                         display: Controls.Button.IconOnly
        //                         icon.name: "folder-open"
        //                         text: i18n("Select")

        //                         onClicked: {
        //                             folderDialogTaildropDirectory.open();
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }
    }
}
