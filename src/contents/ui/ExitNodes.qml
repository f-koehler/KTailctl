// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0
import org.kde.kirigami 2.19 as Kirigami

Kirigami.ScrollablePage {
    id: exit_nodes

    objectName: "Exit Nodes"
    Layout.fillWidth: true
    title: i18n("Exit Nodes")

    Component {
        //     width: ListView.view ? ListView.view.width : implicitWidth
        //     contentItem: RowLayout {
        //         Kirigami.Icon {
        //             source: model.selfHosted ? "network-server-symbolic" : "qrc:/country-flags/"+model.countryCode+".svg"
        //         }
        //         Controls.Label {
        //             text: model.name
        //         }
        //     }
        //     actions: [
        //         Kirigami.Action {
        //             icon.name: model.active ? "dialog-cancel" : "dialog-ok"
        //             text: model.active ? "Unset" : "Use"
        //             onTriggered: model.active ? Util.unsetExitNode() : Util.setExitNode(model.ip)
        //         }
        //     ]
        // }

        id: delegateComponent

        Controls.Label {
            text: "test"
        }
        // Kirigami.SwipeListItem {

    }

    ListView {
        // section {
        //     property: "section"
        //     delegate: Kirigami.ListSectionHeader {
        //         text: section
        //     }
        // }

        model: App.exitNodesModel
        delegate: delegateComponent
    }

}
