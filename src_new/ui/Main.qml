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
                icon.name: "speedometer"
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
                icon.name: "help-about"
                page: "qrc:/ui/pages/About.qml"
                pagePool: mainPagePool
                text: i18n("About")
            },
            Kirigami.Action {
                icon.name: "process-stop"
                text: i18n("Stop tailscale")
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

    Timer {
        interval: 5000
        repeat: true
        running: true
        triggeredOnStart: false

        onTriggered: KTailctl.Tailscale.status.refresh()
    }
}
