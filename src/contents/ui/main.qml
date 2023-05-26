// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18n("KTailctl")

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
        interval: App.config.refreshInterval ? App.config.refreshInterval : 500
        onTriggered: {
            Tailscale.status.refresh();
            //Tailscale.preferences.refresh();
        }
        triggeredOnStart: true
        running: true
        repeat: true
    }

    function navigateTo(name) {
        if(pageStack.layers.currentItem.objectName == name) {
            return;
        }

        while(pageStack.layers.depth > 1) {
            pageStack.layers.pop();
        }

        if(pageStack.layers.currentItem.objectName != name) {
            pageStack.layers.replace("qrc:" + name + ".qml");
        }
    }

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("KTailctl")
        titleIcon: "applications-graphics"
        isMenu: false
        modal: false
        collapsible: false
        collapsed: false
        width: 200
        actions: [
            Kirigami.Action {
                text: i18n("Peers")
                icon.name: "network-wired"
                onTriggered: navigateTo("Peers")
            },
            Kirigami.Action {
                text: i18n("Statistics")
                icon.name: "office-chart-line-stacked"
                onTriggered: navigateTo("Statistics")
            },
            Kirigami.Action {
                text: i18n("Settings")
                icon.name: "settings-configure"
                onTriggered: navigateTo("Settings")
            },
            Kirigami.Action {
                text: i18n("About KTailctl")
                icon.name: "help-about"
                onTriggered: navigateTo("About")
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

    pageStack.initialPage: "qrc:Peers.qml"


}
