// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.fkoehler.KTailctl
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates
import org.kde.config as KConfig

Kirigami.ApplicationWindow {
    id: root

    property string pageName

    function pushPage(page): void {
        if (pageName == page) {
            return;
        }

        var pageObject = Qt.createComponent("org.fkoehler.KTailctl", page);
        if (!pageObject) {
            page = "Peers";
            pageObject = Qt.createComponent("org.fkoehler.KTailctl", page);
        }
        pageStack.clear();
        pageStack.layers.clear();
        pageStack.push(pageObject);
        pageName = page;
    }

    minimumHeight: Kirigami.Units.gridUnit * 20
    minimumWidth: Kirigami.Units.gridUnit * 20
    title: i18n("KTailctl")

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer

    }
    globalDrawer: Kirigami.OverlayDrawer {
        id: drawer

        Kirigami.Theme.colorSet: Kirigami.Theme.View
        Kirigami.Theme.inherit: false
        bottomPadding: 0
        drawerOpen: !Kirigami.Settings.isMobile && enabled
        edge: Qt.application.layoutDirection === Qt.RightToLeft ? Qt.RightEdge : Qt.LeftEdge
        handleClosedIcon.source: modal ? null : "sidebar-expand-left"
        handleOpenIcon.source: modal ? null : "sidebar-collapse-left"
        handleVisible: modal && enabled
        leftPadding: 0
        // Only modal when not collapsed, otherwise collapsed won't show.
        modal: !enabled || Kirigami.Settings.isMobile || Kirigami.Settings.tabletMode || (applicationWindow().width < Kirigami.Units.gridUnit * 50 && !collapsed)
        rightPadding: 0
        topPadding: 0
        width: Kirigami.Units.gridUnit * 16

        contentItem: ColumnLayout {
            spacing: 0

            Controls.ToolBar {
                Layout.bottomMargin: Kirigami.Units.smallSpacing / 2
                Layout.fillWidth: true
                Layout.preferredHeight: pageStack.globalToolBar.preferredHeight
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
                        icon.name: "network-wired"
                        text: i18n("Peers")

                        onTriggered: pushPage("Peers")
                    },
                    Kirigami.Action {
                        icon.name: "computer-symbolic"
                        text: i18n("This PC")

                        onTriggered: pushPage("Self")
                    },
                    Kirigami.Action {
                        icon.name: "globe"
                        text: i18n("Exit Nodes")

                        onTriggered: pushPage("ExitNodes")
                    },
                    // Kirigami.Action {
                    //     icon.name: "office-chart-line-stacked"
                    //     text: i18n("Statistics")

                    //     onTriggered: pushPage("Statistics")
                    // },
                    Kirigami.Action {
                        icon.name: "settings-configure"
                        text: i18n("Settings")

                        onTriggered: pushPage("Settings")
                    },
                    Kirigami.Action {
                        icon.name: "help-about"
                        text: i18n("About KTailctl")

                        onTriggered: pushPage("About")
                    },
                    Kirigami.Action {
                        icon.name: (Tailscale.backendState == "Running") ? "process-stop" : "media-playback-start"
                        text: (Tailscale.backendState == "Running") ? "Stop tailscale" : "Start tailscale"
                        visible: Tailscale.success && Tailscale.isOperator

                        onTriggered: {
                            Tailscale.toggle();
                        }
                    },
                    Kirigami.Action {
                        icon.name: "application-exit"
                        text: i18n("Quit")

                        onTriggered: Qt.quit()
                    }
                ]

                model: actions

                delegate: Delegates.RoundedItemDelegate {
                    required property var modelData

                    Controls.ButtonGroup.group: pageButtonGroup
                    Layout.fillWidth: true
                    action: modelData
                    padding: Kirigami.Units.largeSpacing
                    visible: modelData.visible
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
        Behavior on width {
            NumberAnimation {
                duration: Kirigami.Units.longDuration
                easing.type: Easing.InOutQuad
            }
        }

        onModalChanged: drawerOpen = !modal
    }

    Component.onCompleted: {
        pushPage("Peers");
    }
    onHeightChanged: saveWindowGeometryTimer.restart()
    onWidthChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()

    KConfig.WindowStateSaver {
        configGroupName: "MainWindow"
    }

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
        //Tailscale.preferences.refresh();

        id: refreshStatusTimer

        interval: App.config.refreshInterval ? App.config.refreshInterval : 500
        repeat: true
        running: true
        triggeredOnStart: true

        onTriggered: {
            Tailscale.refresh();
        }
    }
}
