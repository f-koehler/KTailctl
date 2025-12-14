import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl
import QtQuick.Controls
import QtQml

Kirigami.ApplicationWindow {
    id: root

    Label {
        text: Tailscale.status.version
    }

    Timer {
        interval: 5000
        repeat: true
        running: true
        triggeredOnStart: true

        onTriggered: Tailscale.status.refresh()
    }
}
