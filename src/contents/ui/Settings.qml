// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.labs.mobileform 0.1 as MobileForm
import org.kde.kirigamiaddons.labs.components 1.0 as Components
import org.fkoehler.KTailctl 1.0

Kirigami.ScrollablePage {
    id: settings

    objectName: "Settings"
    title: i18n("Settings")

    leftPadding: 0
    rightPadding: 0

    header: Components.Banner {
        width: parent.width
        visible: !Tailscale.status.success
        type: Kirigami.MessageType.Error
        text: i18n("Tailscaled is not running")
    }

    ColumnLayout {
        MobileForm.FormCard {
            Layout.fillWidth: true

            contentItem: ColumnLayout {
                spacing: 0

                MobileForm.FormSpinBoxDelegate {
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

        MobileForm.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing

            title: i18nc("@title:group", "Tailscale")
        }

        MobileForm.FormCard {
            Layout.fillWidth: true

            contentItem: ColumnLayout {
                spacing: 0

                MobileForm.FormSwitchDelegate {
                    id: acceptRoutes

                    text: i18nc("@label", "Accept Routes:")
                    checked: Tailscale.preferences.acceptRoutes
                    onClicked: Tailscale.preferences.acceptRoutes = !Tailscale.preferences.acceptRoutes
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                MobileForm.FormDelegateSeparator { above: acceptDNS; below: acceptRoutes }

                MobileForm.FormSwitchDelegate {
                    id: acceptDNS

                    text: i18nc("@label", "Accept DNS:")
                    checked: Tailscale.preferences.acceptDNS
                    onClicked: Tailscale.preferences.acceptDNS = !Tailscale.preferences.acceptDNS
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                MobileForm.FormDelegateSeparator { above: advertiseExitNode; below: acceptDNS }

                MobileForm.FormSwitchDelegate {
                    id: advertiseExitNode

                    text: i18nc("@label", "Exit node:")
                    checked: Tailscale.preferences.advertiseExitNode
                    onClicked: Tailscale.preferences.advertiseExitNode = !Tailscale.preferences.advertiseExitNode
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                // MobileForm.FormTextFieldDelegaze {
                //     id: textTailscaleHostname
                //     label: i18nc("@label", "Hostname:")
                //     text: Tailscale.preferences.hostname
                //     onEditingFinished: {Tailscale.preferences.hostname = textTailscaleHostname.text; }
                //     enabled: Tailscale.status.isOperator && Tailscale.status.success
                // }

                MobileForm.FormDelegateSeparator { above: shieldsUp; below: advertiseExitNode }

                MobileForm.FormSwitchDelegate {
                    id: shieldsUp

                    text: i18nc("@label", "Shields up:")
                    checked: Tailscale.preferences.shieldsUp
                    onClicked: Tailscale.preferences.shieldsUp = !Tailscale.preferences.shieldsUp
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                MobileForm.FormDelegateSeparator { above: ssh; below: shieldsUp }

                MobileForm.FormSwitchDelegate {
                    id: ssh

                    text: i18nc("@label", "SSH:")
                    checked: Tailscale.preferences.ssh
                    onClicked: Tailscale.preferences.ssh = !Tailscale.preferences.ssh
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }
            }
        }

        MobileForm.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing

            title: i18nc("@title:group", "Taildrop (requires restart)")
        }

        MobileForm.FormCard {
            Layout.fillWidth: true

            contentItem: ColumnLayout {
                spacing: 0

                MobileForm.FormSwitchDelegate {
                    id: status
                    text: i18nc("@label", "Status:")
                    checked: App.config.taildropEnabled
                    onClicked: {
                        App.config.taildropEnabled = !App.config.taildropEnabled;
                        App.config.save();
                    }
                    enabled: Tailscale.status.isOperator && Tailscale.status.success
                }

                MobileForm.FormDelegateSeparator { above: status }

                MobileForm.AbstractFormDelegate {
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

        // Controls.ComboBox {
        //     id: comboTaildropStrategy
        //     Kirigami.FormData.label: "Strategy:"
        //     model: ["Rename", "Overwrite", "Skip"]
        //     onActivated: {
        //         App.config.taildropStrategy = comboTaildropStrategy.currentText;
        //         App.config.save()
        //     }
        //     enabled: Tailscale.status.isOperator
        // }

        MobileForm.FormHeader {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing

            title: i18nc("@title:group", "Interface")
        }

        MobileForm.FormCard {
            Layout.fillWidth: true

            contentItem: ColumnLayout {
                spacing: 0

                MobileForm.FormSwitchDelegate {
                    text: i18nc("@label", "Start minimized:")
                    checked: App.config.startMinimized
                    onClicked: {
                        App.config.startMinimized = !App.config.startMinimized;
                        App.config.save();
                    }
                }
            }
        }
    }
}
