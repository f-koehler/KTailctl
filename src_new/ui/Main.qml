import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml
import org.kde.kirigami.platform

Kirigami.ApplicationWindow {
    id: root

    pageStack.initialPage: Kirigami.ScrollablePage {
        Layout.fillWidth: true

        ListView {
            anchors.fill: parent
            model: Tailscale.status.peers

            delegate: ItemDelegate {
                width: ListView.view.width

                // text: dnsName
                contentItem: RowLayout {
                    ToolButton {
                        ToolTip.delay: Units.toolTipDelay
                        ToolTip.text: "Show details"
                        ToolTip.visible: hovered
                        icon.name: "view-list-details"
                    }

                    ToolButton {
                        ToolTip.delay: Units.toolTipDelay
                        ToolTip.text: "More actions"
                        ToolTip.visible: hovered
                        icon.name: "menu_new"
                    }

                    ToolButton {
                        ToolTip.delay: Units.toolTipDelay
                        ToolTip.text: "Copy DNS name to clipboard"
                        ToolTip.visible: hovered
                        icon.name: "edit-copy"
                        text: dnsName
                    }

                    ToolButton {
                        ToolTip.delay: Units.toolTipDelay
                        ToolTip.text: "Copy IP address to clipboard"
                        ToolTip.visible: hovered
                        icon.name: "edit-copy"
                        text: tailscaleIps[0]
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Kirigami.Icon {
                        source: online ? "online" : "offline"
                    }

                    Item {
                        width: Units.largeSpacing
                    }
                }
            }
        }

        // ColumnLayout {
        //     Repeater {
        //         model: Tailscale.status.peers
        //
        //         Label {
        //             id: delegate
        //
        //             // required property PeerStatus modelData
        //             // property alias peer: delegate.modelData
        //
        //             text: dnsName
        //         }
        //     }
        // }
    }

    Timer {
        interval: 5000
        repeat: true
        running: true
        triggeredOnStart: false

        onTriggered: Tailscale.status.refresh()
    }
}
