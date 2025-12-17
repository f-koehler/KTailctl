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
                page: "qrc:/ui/PagePeerList.qml"
                pagePool: mainPagePool
                text: "Peers"
            },
            Kirigami.PagePoolAction {
                icon.name: "help-about"
                page: "qrc:/ui/PageAbout.qml"
                pagePool: mainPagePool
                text: "About"
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
