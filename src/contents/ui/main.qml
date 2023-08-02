// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.delegates 1.0 as Delegates
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
        if(pageStack.currentItem.objectName == name) {
            return;
        }

        while(pageStack.depth > 1) {
            pageStack.pop();
        }

        if(pageStack.currentItem.objectName != name) {
            pageStack.replace("qrc:" + name + ".qml");
        }
    }

    globalDrawer: Kirigami.OverlayDrawer {
        id: drawer

        edge: Qt.application.layoutDirection === Qt.RightToLeft ? Qt.RightEdge : Qt.LeftEdge

        // Only modal when not collapsed, otherwise collapsed won't show.
        modal: !enabled || Kirigami.Settings.isMobile || Kirigami.Settings.tabletMode || (applicationWindow().width < Kirigami.Units.gridUnit * 50 && !collapsed)

        drawerOpen: !Kirigami.Settings.isMobile && enabled
        width: Kirigami.Units.gridUnit * 16

        Behavior on width {
            NumberAnimation {
                duration: Kirigami.Units.longDuration
                easing.type: Easing.InOutQuad
            }
        }

        Kirigami.Theme.colorSet: Kirigami.Theme.View
        Kirigami.Theme.inherit: false

        handleClosedIcon.source: modal ? null : "sidebar-expand-left"
        handleOpenIcon.source: modal ? null : "sidebar-collapse-left"
        handleVisible: modal && enabled
        onModalChanged: drawerOpen = !modal;

        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        contentItem: ColumnLayout {
            spacing: 0

            Controls.ToolBar {
                Layout.fillWidth: true
                Layout.preferredHeight: pageStack.globalToolBar.preferredHeight
                Layout.bottomMargin: Kirigami.Units.smallSpacing / 2

                leftPadding: Kirigami.Units.largeSpacing
                rightPadding: Kirigami.Units.largeSpacing

                contentItem: Kirigami.Heading {
                    text: i18n("KTailctl")
                }
            }

            Controls.ButtonGroup {
                id: pageButtonGroup
            }

            Repeater {
                readonly property list<Kirigami.Action> actions: [
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

                model: actions
                delegate: Delegates.RoundedItemDelegate {
                    required property var modelData
                    Controls.ButtonGroup.group: pageButtonGroup

                    padding: Kirigami.Units.largeSpacing
                    action: modelData
                    Layout.fillWidth: true
                    visible: modelData.visible
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    pageStack.initialPage: "qrc:Peers.qml"
}
