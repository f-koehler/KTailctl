import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl

Kirigami.ScrollablePage {
    id: page

    Layout.fillWidth: true

    actions: [
        Kirigami.Action {
            id: customAction

            icon.name: "search"
            text: "Custom Component"

            displayComponent: Kirigami.SearchField {
            }
        }
    ]

    ListView {
        anchors.fill: parent
        model: KTailctl.Tailscale.status.peers

        delegate: ItemDelegate {
            width: ListView.view.width
            id: delegate

            contentItem: RowLayout {
                Kirigami.Icon {
                    ToolTip.delay: Kirigami.Units.toolTipDelay
                    ToolTip.text: online ? "Online" : "Offline"
                    ToolTip.visible: hovered
                    source: online ? "online" : "offline"
                }

                ToolButton {
                    ToolTip.delay: Kirigami.Units.toolTipDelay
                    ToolTip.text: "Copy DNS name to clipboard"
                    ToolTip.visible: hovered
                    icon.name: "edit-copy"
                    text: dnsName
                    onClicked: KTailctl.Util.setClipboardText(dnsName)
                }

                ToolButton {
                    ToolTip.delay: Kirigami.Units.toolTipDelay
                    ToolTip.text: "Copy IP address to clipboard"
                    ToolTip.visible: hovered
                    icon.name: "edit-copy"
                    text: tailscaleIps[0]
                    onClicked: KTailctl.Util.setClipboardText(tailscaleIps[0])
                }

                Item {
                    Layout.fillWidth: true
                }

                ToolButton {
                    ToolTip.delay: Kirigami.Units.toolTipDelay
                    ToolTip.text: "More actions"
                    ToolTip.visible: hovered
                    icon.name: "menu_new"
                    onClicked: menu.open()

                    Menu {
                        id: menu
                        MenuItem {
                            icon.name: "icon_details"
                            text: "Node info"
                            onClicked: root.pageStack.layers.push(pagePeerInfo)
                        }
                    }
                }

                Item {
                    width: Kirigami.Units.largeSpacing
                }
            }
        }
    }
    Component {
        id: pagePeerInfo
        PeerInfo {}
    }
}
