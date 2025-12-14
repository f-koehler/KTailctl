import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml

Kirigami.ApplicationWindow {
    id: root

    ColumnLayout {
        Repeater {
            model: Tailscale.status.peers

            Label {
                id: delegate

                // required property PeerStatus modelData
                // property alias peer: delegate.modelData

                text: dnsName
            }
        }
    }

    Timer {
        interval: 5000
        repeat: true
        running: true
        triggeredOnStart: false

        onTriggered: Tailscale.status.refresh()
    }
}
