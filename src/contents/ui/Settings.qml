// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.0
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
                Config.refreshInterval = spinRefreshInterval.value;
                Config.save();
            }
            Kirigami.FormData.label: i18n("Refresh rate (ms)")
        }

        Flow {
            Kirigami.FormData.label: i18n("Tailscale executable")

            Controls.TextField {
                id: textTailscaleExecutable
                text: Config.tailscaleExecutable

                onEditingFinished: {
                    Config.tailscaleExecutable = textTailscaleExecutable.text;
                    Config.save();
                }
            }

            FileDialog {
                id: fileDialogTailscaleExecutable
                title: "Please select tailscale executable"
                selectMultiple: false
                onAccepted: {
                    Config.tailscaleExecutable = fileDialogTailscaleExecutable.fileUrls[0];
                }
            }

            Controls.Button {
                text: "…"
                onClicked: {
                    fileDialogTailscaleExecutable.open()
                }
            }
        }
    }
}
