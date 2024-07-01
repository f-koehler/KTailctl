// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Dialogs
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard
import org.kde.kirigamiaddons.labs.components 1.0 as Components

Kirigami.ScrollablePage {
    id: settings

    objectName: "Settings"
    title: i18n("Settings")
    leftPadding: 0
    rightPadding: 0
    actions: [
        Kirigami.Action {
            visible: Tailscale.success && Tailscale.isOperator
            text: (Tailscale.backendState == "Running") ? "Stop tailscale" : "Start tailscale"
            icon.name: (Tailscale.backendState == "Running") ? "process-stop" : "media-playback-start"
            onTriggered: {
                App.tailscale.toggle();
            }
        }
    ]

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

                    text: i18nc("@label", "Start minimized:")
                    checked: App.config.startMinimized
                    onClicked: {
                        App.config.startMinimized = !App.config.startMinimized;
                        App.config.save();
                    }
                    enabled: App.config.enableTrayIcon
                }

                FormCard.FormDelegateSeparator {
                    below: startMinimized
                    above: enableTrayIcon
                }

                FormCard.FormSwitchDelegate {
                    id: enableTrayIcon

                    text: i18nc("@label", "Enable tray icon:")
                    checked: App.config.enableTrayIcon
                    onClicked: {
                        App.config.enableTrayIcon = !App.config.enableTrayIcon;
                        if (!App.config.enableTrayIcon)
                            App.config.startMinimized = false;

                        App.config.save();
                        App.trayIcon.visible = App.config.enableTrayIcon;
                    }
                }

                FormCard.FormDelegateSeparator {
                    below: enableTrayIcon
                    above: trayIconStyle
                }

                FormCard.FormComboBoxDelegate {
                    id: trayIconStyle

                    text: i18nc("@label", "Tray icon style:")
                    model: ["Colorful", "Breeze Dark", "Breeze Light"]
                    displayText: App.config.trayIconStyle
                    onActivated: {
                        App.config.trayIconStyle = trayIconStyle.currentText;
                        App.config.save();
                    }
                }

                FormCard.FormDelegateSeparator {
                    below: trayIconStyle
                    above: spinRefreshInterval
                }

                FormCard.FormSpinBoxDelegate {
                    id: spinRefreshInterval

                    from: 10
                    to: 10000
                    value: refreshStatusTimer.interval
                    stepSize: 100
                    onValueChanged: {
                        refreshStatusTimer.interval = spinRefreshInterval.value;
                        App.config.refreshInterval = spinRefreshInterval.value;
                        App.config.save();
                    }
                    label: i18nc("@label", "Refresh rate (ms):")
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

                    text: i18nc("@label", "Enable Tailscale:")
                    checked: Tailscale.backendState == "Running"
                    onClicked: {
                        App.tailscale.toggle();
                    }
                    enabled: Tailscale.isOperator && Tailscale.success
                }

                FormCard.FormSwitchDelegate {
                    id: acceptRoutes

                    text: i18nc("@label", "Accept Routes:")
                    checked: Preferences.acceptRoutes
                    onClicked: Preferences.acceptRoutes = !Preferences.acceptRoutes
                    enabled: Tailscale.isOperator && Tailscale.success
                }

                FormCard.FormDelegateSeparator {
                    above: acceptDNS
                    below: acceptRoutes
                }

                FormCard.FormSwitchDelegate {
                    id: acceptDNS

                    text: i18nc("@label", "Accept DNS:")
                    checked: Preferences.acceptDNS
                    onClicked: Preferences.acceptDNS = !Preferences.acceptDNS
                    enabled: Tailscale.isOperator && Tailscale.success
                }

                FormCard.FormDelegateSeparator {
                    above: textTailscaleHostname
                    below: acceptDNS
                }

                FormCard.FormTextFieldDelegate {
                    id: textTailscaleHostname

                    label: i18nc("@label", "Hostname:")
                    text: Preferences.hostname
                    onEditingFinished: {
                        Preferences.hostname = textTailscaleHostname.text;
                    }
                    enabled: Tailscale.isOperator && Tailscale.success
                }

                FormCard.FormDelegateSeparator {
                    above: shieldsUp
                    below: textTailscaleHostname
                }

                FormCard.FormSwitchDelegate {
                    id: shieldsUp

                    text: i18nc("@label", "Shields up:")
                    checked: Preferences.shieldsUp
                    onClicked: Preferences.shieldsUp = !Preferences.shieldsUp
                    enabled: Tailscale.isOperator && Tailscale.success
                }

                FormCard.FormDelegateSeparator {
                    above: ssh
                    below: shieldsUp
                }

                FormCard.FormSwitchDelegate {
                    id: ssh

                    text: i18nc("@label", "SSH:")
                    checked: Preferences.ssh
                    onClicked: Preferences.ssh = !Preferences.ssh
                    enabled: Tailscale.isOperator && Tailscale.success
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

                    text: i18nc("@label", "Run exit node:")
                    checked: Preferences.advertiseExitNode
                    onClicked: {
                        if (!Preferences.advertiseExitNode) {
                            Util.unsetExitNode();
                            comboExitNode.currentIndex = 0;
                            Preferences.advertiseExitNode = true;
                        } else {
                            Preferences.advertiseExitNode = false;
                        }
                    }
                    enabled: Tailscale.isOperator && Tailscale.success
                }
            }

        }

        FormCard.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            title: i18nc("@title:group", "Taildrop (requires restart)")
        }

        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0

                FormCard.FormSwitchDelegate {
                    id: status

                    text: i18nc("@label", "Status:")
                    checked: App.config.taildropEnabled
                    onClicked: {
                        App.config.taildropEnabled = !App.config.taildropEnabled;
                        App.config.save();
                    }
                    enabled: Tailscale.isOperator && Tailscale.success
                }

                FormCard.FormDelegateSeparator {
                    above: status
                }

                FormCard.AbstractFormDelegate {
                    enabled: Tailscale.isOperator && Tailscale.success

                    contentItem: ColumnLayout {
                        Controls.Label {
                            text: i18nc("@label", "Directory")
                        }

                        RowLayout {
                            Controls.TextField {
                                id: textTaildropDirectory

                                text: App.config.taildropDirectory
                                Layout.fillWidth: true
                                onEditingFinished: {
                                    App.config.taildropDirectory = textTaildropDirectory.text;
                                    App.config.save();
                                }
                            }

                            FolderDialog {
                                id: folderDialogTaildropDirectory

                                currentFolder: App.config.taildropDirectory
                                onAccepted: {
                                    App.config.taildropDirectory = App.fileUrlToString(folderDialogTaildropDirectory.fileUrls[0]);
                                    App.config.save();
                                }
                            }

                            Controls.Button {
                                icon.name: "folder-open"
                                text: i18n("Select")
                                display: Controls.Button.IconOnly
                                onClicked: {
                                    folderDialogTaildropDirectory.open();
                                }
                                Controls.ToolTip.text: text
                                Controls.ToolTip.visible: hovered
                                Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                            }

                        }

                    }

                }

            }

        }

    }

    header: Components.Banner {
        width: parent.width
        visible: !Tailscale.success
        type: Kirigami.MessageType.Error
        text: i18n("Tailscaled is not running")
    }

}
