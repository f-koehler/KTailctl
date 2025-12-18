import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import QtQml.Models as Models

Kirigami.ScrollablePage {
    Layout.fillWidth: true

    Models.SortFilterProxyModel {
        id: exitNodeModel
        model: KTailctl.Tailscale.status.peers
        filters: [
            Models.ValueFilter {
                roleName: "exitNodeOption"
                value: true
            }
        ]
    }

    ListView {
        anchors.fill: parent
        model: exitNodeModel

        delegate: ItemDelegate {
            width: ListView.view.width
            id: delegate

            // text: dnsName
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
                }

                Item {
                    width: Kirigami.Units.largeSpacing
                }
            }
        }
    }
}
