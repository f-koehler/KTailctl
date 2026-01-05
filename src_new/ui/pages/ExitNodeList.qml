import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import QtQml.Models as Models

Kirigami.ScrollablePage {
    Layout.fillWidth: true

    Component {
        id: pagePeerInfo

        PeerInfo {}
    }

    Models.SortFilterProxyModel {
        id: exitNodeModel
        model: KTailctl.Tailscale.status.peers
        filters: [
            Models.ValueFilter {
                roleName: "exitNodeOption"
                value: true
            },
            Models.ValueFilter {
                roleName: "online"
                value: true
            },
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
                    ToolTip.text: "Country"
                    ToolTip.visible: hovered
                    source: "flag"
                }

                Label {
                    text: dnsName
                }

                Item {
                    Layout.fillWidth: true
                }

                ToolButton {
                    ToolTip.delay: Kirigami.Units.toolTipDelay
                    ToolTip.text: "Enable exit node"
                    ToolTip.visible: hovered
                    icon.name: "system-switch-user"
                }

                ToolButton {
                    ToolTip.delay: Kirigami.Units.toolTipDelay
                    ToolTip.text: "View node info"
                    ToolTip.visible: hovered
                    icon.name: "help-info"
                    onClicked: applicationWindow().pageStack.layers.push(pagePeerInfo, {
                        peer: KTailctl.Tailscale.status.peerWithId(id)
                    })
                }

                Item {
                    width: Kirigami.Units.largeSpacing
                }
            }
        }
    }
}
