// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0

Kirigami.ScrollablePage {
    id: statistics
    objectName: "Statistics"

    Layout.fillWidth: true

    title: i18n("Statistics")

    Kirigami.FormLayout {
        anchors.fill: parent

        Controls.Label {
            text: Util.formatSpeedHumanReadable(Tailscale.statistics.totalUpSpeed.average1Second)
        }
    }
}
