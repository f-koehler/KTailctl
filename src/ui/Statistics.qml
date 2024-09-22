// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0
import org.fkoehler.KTailctl.Components 1.0 as KTailctlComponents
import org.kde.kirigami 2.19 as Kirigami

Kirigami.ScrollablePage {
    id: statistics

    Layout.fillWidth: true
    objectName: "Statistics"
    title: i18n("Statistics")

    header: KTailctlComponents.HeaderBanner {
    }

    Kirigami.FormLayout {
        anchors.fill: parent

        Controls.Label {
            Kirigami.FormData.label: "Total upload speed"
            text: Util.formatSpeedHumanReadable(Tailscale.statistics.totalUpSpeed.average1Second)
        }

        Controls.Label {
            Kirigami.FormData.label: "Total download speed"
            text: Util.formatSpeedHumanReadable(Tailscale.statistics.totalDownSpeed.average1Second)
        }
    }
}
