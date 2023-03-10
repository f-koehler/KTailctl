// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.Tailctl 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Tailctl")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    onClosing: App.saveWindowGeometry(root)

    onWidthChanged: saveWindowGeometryTimer.restart()
    onHeightChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()

    Component.onCompleted: App.restoreWindowGeometry(root)

    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        onTriggered: App.saveWindowGeometry(root)
    }

    Timer {
        id: refreshStatusTimer
        interval: Config.refreshInterval ? Config.refreshInterval : 500
        onTriggered: App.refreshStatus(root, Config.tailscaleExecutable)
        triggeredOnStart: true
        running: true
        repeat: true
    }

    property string backend_state: "Unknown"

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("Tailctl")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("Settings")
                icon.name: "settings-configure"
                onTriggered: pageStack.layers.push('qrc:Settings.qml')
            },
            Kirigami.Action {
                text: i18n("About Tailctl")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    pageStack.initialPage: peers

    Kirigami.ScrollablePage {
        id: peers

        Layout.fillWidth: true

        title: i18n("Peers")

        Kirigami.CardsListView {
            id: listPeers
            model: ModelPeers
            delegate: delegatePeers
        }
    }

    Component {
        id: delegatePeers
        Kirigami.AbstractCard {
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout

                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                    }

                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.smallSpacing
                    columns: 3

                    Controls.Label {
                        text: host_name
                    }
                }
            }
        }
    }
}
