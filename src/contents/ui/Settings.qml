// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Dialogs 1.3
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

    ColumnLayout {
        // Controls.ComboBox {
        //     id: comboTaildropStrategy
        //     Kirigami.FormCard.label: "Strategy:"
        //     model: ["Rename", "Overwrite", "Skip"]
        //     onActivated: {
        //         App.config.taildropStrategy = comboTaildropStrategy.currentText;
        //         App.config.save()
        //     }
        //     enabled: Tailscale.status.isOperator
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
                    below: enableTrayIcon
                    above: iconTheme
                }

                FormCard.FormComboBoxDelegate {
                    id: iconTheme

                    text: i18nc("@label", "Icon theme:")
                    model: IconThemeModel
                    textRole: "display"
                    displayText: App.config.iconTheme
                    onActivated: {
                        Util.setIconTheme(iconTheme.currentText);
                    }
                }

                FormCard.FormDelegateSeparator {
                    below: iconTheme
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
                    checked: Tailscale.status.backendState == "Running"
                    onClicked: {
                        App.tailscale.toggle();
                    }
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                FormCard.FormSwitchDelegate {
                    id: acceptRoutes

                    text: i18nc("@label", "Accept Routes:")
                    checked: Tailscale.preferences.acceptRoutes
                    onClicked: Tailscale.preferences.acceptRoutes = !Tailscale.preferences.acceptRoutes
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                FormCard.FormDelegateSeparator {
                    above: acceptDNS
                    below: acceptRoutes
                }

                FormCard.FormSwitchDelegate {
                    id: acceptDNS

                    text: i18nc("@label", "Accept DNS:")
                    checked: Tailscale.preferences.acceptDNS
                    onClicked: Tailscale.preferences.acceptDNS = !Tailscale.preferences.acceptDNS
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                FormCard.FormDelegateSeparator {
                    above: textTailscaleHostname
                    below: acceptDNS
                }

                FormCard.FormTextFieldDelegate {
                    id: textTailscaleHostname

                    label: i18nc("@label", "Hostname:")
                    text: Tailscale.preferences.hostname
                    onEditingFinished: {
                        Tailscale.preferences.hostname = textTailscaleHostname.text;
                    }
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                FormCard.FormDelegateSeparator {
                    above: shieldsUp
                    below: textTailscaleHostname
                }

                FormCard.FormSwitchDelegate {
                    id: shieldsUp

                    text: i18nc("@label", "Shields up:")
                    checked: Tailscale.preferences.shieldsUp
                    onClicked: Tailscale.preferences.shieldsUp = !Tailscale.preferences.shieldsUp
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                FormCard.FormDelegateSeparator {
                    above: ssh
                    below: shieldsUp
                }

                FormCard.FormSwitchDelegate {
                    id: ssh

                    text: i18nc("@label", "SSH:")
                    checked: Tailscale.preferences.ssh
                    onClicked: Tailscale.preferences.ssh = !Tailscale.preferences.ssh
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
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
                    checked: Tailscale.preferences.advertiseExitNode
                    onClicked: {
                        if (!Tailscale.preferences.advertiseExitNode) {
                            Util.unsetExitNode();
                            comboExitNode.currentIndex = 0;
                            Tailscale.preferences.advertiseExitNode = true;
                        } else {
                            Tailscale.preferences.advertiseExitNode = false;
                        }
                    }
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                FormCard.AbstractFormDelegate {

                    contentItem: ColumnLayout {
                        RowLayout {
                            Controls.Label {
                                text: i18nc("@label", "Exit node:")
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Controls.ComboBox {
                                id: comboExitNode

                                model: App.exitNodeModel
                                textRole: "hostName"
                                valueRole: "ip"
                                currentIndex: {
                                    if (!Tailscale.status.currentExitNode)
                                        return -1;

                                    if (Tailscale.status.currentExitNode.tailscaleIps.size < 1)
                                        return -1;

                                    return indexOfValue(Tailscale.status.currentExitNode.tailscaleIps[0]);
                                }
                                onActivated: function(index) {
                                    Util.setExitNode(currentValue);
                                }
                            }

                            Controls.Button {
                                text: i18n("Unset")
                                display: Controls.Button.IconOnly
                                icon.name: "dialog-close"
                                Controls.ToolTip.text: text
                                Controls.ToolTip.visible: hovered
                                Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                                onClicked: {
                                    Util.unsetExitNode();
                                    comboExitNode.currentIndex = -1;
                                }
                            }

                        }

                    }

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
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                FormCard.FormDelegateSeparator {
                    above: status
                }

                FormCard.AbstractFormDelegate {
                    enabled: Tailscale.status.isOperator && Tailscale.status.success

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

                            FileDialog {
                                id: folderDialogTaildropDirectory

                                folder: App.config.taildropDirectory
                                selectMultiple: false
                                // TODO: with Qt6 we can use the FolderDialog type instead
                                selectFolder: true
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
        visible: !Tailscale.status.success
        type: Kirigami.MessageType.Error
        text: i18n("Tailscaled is not running")
    }

    actions.main: Kirigami.Action {
        visible: Tailscale.status.success && Tailscale.status.isOperator
        text: (Tailscale.status.backendState == "Running") ? "Stop tailscale" : "Start tailscale"
        icon.name: (Tailscale.status.backendState == "Running") ? "process-stop" : "media-playback-start"
        onTriggered: {
            App.tailscale.toggle();
        }
    }

}
