// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0

Kirigami.Page {
    id: settings
    objectName: "Settings"

    Layout.fillWidth: true

    title: i18n("Settings")

    Kirigami.FormLayout {
        anchors.fill: parent

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "KTailctl"
        }

        Controls.SpinBox {
            id: spinRefreshInterval
            from: 10
            to: 10000
            value: refreshStatusTimer.interval
            stepSize: 100
            validator: IntValidator {
                bottom: Math.min(spinRefreshInterval.from, spinRefreshInterval.to)
                top: Math.max(spinRefreshInterval.from, spinRefreshInterval.to)
            }
            onValueModified: {
                refreshStatusTimer.interval = spinRefreshInterval.value;
                App.config.refreshInterval = spinRefreshInterval.value;
                App.config.save();
            }
            Kirigami.FormData.label: i18n("Refresh rate (ms):")
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Tailscale"
            visible: Tailscale.status.isOperator
        }

        Controls.Switch {
            Kirigami.FormData.label: "Accept Routes:"
            checked: Tailscale.preferences.acceptRoutes
            onClicked: Tailscale.preferences.acceptRoutes = !Tailscale.preferences.acceptRoutes
            visible: Tailscale.status.isOperator
        }

        Controls.Switch {
            Kirigami.FormData.label: "Accept DNS:"
            checked: Tailscale.preferences.acceptDNS
            onClicked: Tailscale.preferences.acceptDNS = !Tailscale.preferences.acceptDNS
            visible: Tailscale.status.isOperator
        }

        Controls.Switch {
            Kirigami.FormData.label: "Exit node:"
            checked: Tailscale.preferences.advertiseExitNode
            onClicked: Tailscale.preferences.advertiseExitNode = !Tailscale.preferences.advertiseExitNode
            visible: Tailscale.status.isOperator
        }

        // Controls.TextField {
        //     id: textTailscaleHostname
        //     Kirigami.FormData.label: "Hostname:"
        //     text: Tailscale.preferences.hostname
        //     onEditingFinished: {Tailscale.preferences.hostname = textTailscaleHostname.text; }
        //     visible: Tailscale.status.isOperator
        // }

        Controls.Switch {
            Kirigami.FormData.label: "Shields up:"
            checked: Tailscale.preferences.shieldsUp
            onClicked: Tailscale.preferences.shieldsUp = !Tailscale.preferences.shieldsUp
            visible: Tailscale.status.isOperator
        }

        Controls.Switch {
            Kirigami.FormData.label: "SSH:"
            checked: Tailscale.preferences.ssh
            onClicked: Tailscale.preferences.ssh = !Tailscale.preferences.ssh
            visible: Tailscale.status.isOperator
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Taildrop (requires restart)"
            visible: Tailscale.status.isOperator
        }

        Controls.Switch {
            Kirigami.FormData.label: "Status:"
            checked: App.config.taildropEnabled
            onClicked: {
                App.config.taildropEnabled = !App.config.taildropEnabled;
                App.config.save();
            }
            visible: Tailscale.status.isOperator
        }

        Flow {
            Kirigami.FormData.label: "Directory:"
            visible: Tailscale.status.isOperator

            Controls.TextField {
                id: textTaildropDirectory
                text: App.config.taildropDirectory
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
                text: "Select"
                onClicked: {
                    folderDialogTaildropDirectory.open();
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
        //     visible: Tailscale.status.isOperator
        // }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Interface"
        }

        Controls.Switch {
            Kirigami.FormData.label: "Start minimized:"
            checked: App.config.startMinimized
            onClicked: {
                App.config.startMinimized = !App.config.startMinimized;
                App.config.save();
            }
        }
    }
}
