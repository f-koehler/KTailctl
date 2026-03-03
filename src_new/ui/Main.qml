import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl

Kirigami.ApplicationWindow {
    id: root

    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer

        collapsed: true
        collapsible: true
        modal: false
        title: "KTailctl"
        titleIcon: ":/icons/logo.svg"

        actions: [
            Kirigami.PagePoolAction {
                icon.name: "config-users"
                page: "qrc:/ui/pages/LoginProfileList.qml"
                pagePool: mainPagePool
                text: i18n("Login Profiles")
            },
            Kirigami.PagePoolAction {
                icon.name: "distribute-graph-directed"
                page: "qrc:/ui/pages/PeerList.qml"
                pagePool: mainPagePool
                text: i18n("Peers")
            },
            Kirigami.PagePoolAction {
                icon.name: "globe"
                page: "qrc:/ui/pages/ExitNodeList.qml"
                pagePool: mainPagePool
                text: i18n("Exit Nodes")
            },
            Kirigami.PagePoolAction {
                icon.name: "settings"
                page: "qrc:/ui/pages/Settings.qml"
                pagePool: mainPagePool
                text: i18n("Settings")
            },
            Kirigami.PagePoolAction {
                icon.name: "help-about"
                page: "qrc:/ui/pages/About.qml"
                pagePool: mainPagePool
                text: i18n("About")
            },
            Kirigami.Action {
                visible: (KTailctl.Tailscale.status.backendState != KTailctl.Status.Starting) && (KTailctl.Tailscale.status.backendState != KTailctl.Status.Running)
                icon.name: "media-playback-start"
                text: i18n("Start Tailscale")
                onTriggered: KTailctl.Tailscale.up()
            },
            Kirigami.Action {
                visible: (KTailctl.Tailscale.status.backendState == KTailctl.Status.Starting) || (KTailctl.Tailscale.status.backendState == KTailctl.Status.Running)
                icon.name: "process-stop"
                text: i18n("Stop Tailscale")
                onTriggered: KTailctl.Tailscale.down()
            },
            Kirigami.Action {
                icon.name: "application-exit"
                text: i18n("Quit")

                onTriggered: Qt.quit()
            }
        ]
    }

    Kirigami.PagePool {
        id: mainPagePool
    }

    Component.onCompleted: {
        mainPagePool.loadPage("qrc:/ui/pages/LoginProfileList.qml");
        mainPagePool.loadPage("qrc:/ui/pages/PeerList.qml");
        mainPagePool.loadPage("qrc:/ui/pages/ExitNodeList.qml");
        mainPagePool.loadPage("qrc:/ui/pages/Settings.qml");
        mainPagePool.loadPage("qrc:/ui/pages/About.qml");
        pageStack.replace(mainPagePool.loadPage("qrc:/ui/pages/PeerList.qml"));
    }

    Timer {
        interval: KTailctl.Config.refreshInterval
        repeat: true
        running: true
        triggeredOnStart: true

        onTriggered: KTailctl.Tailscale.status.refresh()
    }

    Timer {
        interval: KTailctl.Config.refreshInterval
        repeat: true
        running: true
        triggeredOnStart: true

        onTriggered: KTailctl.Tailscale.preferences.refresh()
    }

    Timer {
        interval: KTailctl.Config.refreshInterval
        repeat: true
        running: true
        triggeredOnStart: true

        onTriggered: KTailctl.Tailscale.refreshLoginProfiles()
    }
}
