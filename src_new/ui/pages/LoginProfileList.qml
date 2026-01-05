import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl

Kirigami.ScrollablePage {
    Layout.fillWidth: true

    ListView {
        anchors.fill: parent
        model: KTailctl.Tailscale.loginProfiles

        delegate: ItemDelegate {
            width: ListView.view.width
            id: delegate

            contentItem: Label {
                text: name
            }
        }
    }
}