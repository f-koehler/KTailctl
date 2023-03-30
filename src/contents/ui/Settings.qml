// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import org.kde.kirigami 2.19 as Kirigami
import org.kde.Tailctl 1.0

Kirigami.Page {
    id: settings

    Layout.fillWidth: true

    title: i18n("Settings")

    Kirigami.FormLayout {
        anchors.fill: parent

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Tailctl"
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

        Flow {
            Kirigami.FormData.label: i18n("Tailscale executable:")

            Controls.TextField {
                id: textTailscaleExecutable
                text: App.config.tailscaleExecutable

                onEditingFinished: {
                    App.config.tailscaleExecutable = textTailscaleExecutable.text;
                    App.config.save();
                }
            }

            FileDialog {
                id: fileDialogTailscaleExecutable
                title: "Please select tailscale executable"
                selectMultiple: false
                onAccepted: {
                    App.config.tailscaleExecutable = App.fileUrlToString(fileDialogTailscaleExecutable.fileUrls[0]);
                    App.config.save();
                }
            }

            Controls.Button {
                icon.name: "document-open"
                text: "Select"
                onClicked: {
                    fileDialogTailscaleExecutable.open();
                }
            }
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "Taildrop"
        }

        Controls.Switch {
            Kirigami.FormData.label: "Status:"
            checked: App.config.taildropEnabled
            onClicked: {
                App.config.taildropEnabled = !App.config.taildropEnabled;
                App.config.save();
            }
        }

        Flow {
            Kirigami.FormData.label: "Directory:"

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

        Controls.ComboBox {
            id: comboTaildropStrategy
            Kirigami.FormData.label: "Strategy:"
            model: ["Rename", "Overwrite", "Skip"]
            onActivated: {
                if(comboTaildropStrategy.currentText == "Rename") {
                    App.config.taildropStrategy = "Rename";
                    App.config.save();
                }
                console.log(comboTaildropStrategy.currentText);
            }
        }
    }
}
