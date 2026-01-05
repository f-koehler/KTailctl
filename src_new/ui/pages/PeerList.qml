import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import QtQml.Models
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl

Kirigami.ScrollablePage {
    id: page

    Layout.fillWidth: true

    property bool filterMullvadEnabled: true
    property bool filterMullvadValue: false
    property bool filterOnlineEnabled: false
    property bool filterOnlineValue: true

    actions: [
        Kirigami.Action {
            id: actionFilterOnline
            visible: page.filterOnlineEnabled
            displayComponent: Kirigami.Chip {
                text: page.filterOnlineValue ? "Online" : "Offline"
                onRemoved: {
                    page.filterOnlineEnabled = false;
                }
                onClicked: {
                    page.filterOnlineValue = !page.filterOnlineValue;
                }
            }
        },
        Kirigami.Action {
            id: actionFilterMullvad
            visible: page.filterMullvadEnabled
            displayComponent: Kirigami.Chip {
                text: page.filterMullvadValue ? "Mullvad" : "Not Mullvad"
                onRemoved: {
                    page.filterMullvadEnabled = false;
                }
                onClicked: {
                    page.filterMullvadValue = !page.filterMullvadValue;
                }
            }
        },
        Kirigami.Action {
            id: actionFilter
            icon.name: "filter"

            Kirigami.Action {
                text: "Online"
                checkable: true
                checked: page.filterOnlineEnabled
                icon.name: "online"
                onToggled: {
                    page.filterOnlineEnabled = !page.filterOnlineEnabled;
                }
            }

            Kirigami.Action {
                text: "Mullvad"
                checkable: true
                checked: page.filterMullvadEnabled
                icon.name: "globe"
                onToggled: {
                    page.filterMullvadEnabled = !page.filterMullvadEnabled;
                }
            }
        }
        // Kirigami.Action {
        //     id: customAction
        //
        //     icon.name: "search"
        //     text: "Custom Component"
        //
        //     displayComponent: Kirigami.SearchField {
        //     }
        // }
    ]

    Component {
        id: pagePeerInfo

        PeerInfo {
        }
    }

    SortFilterProxyModel {
        id: peerModel
        model: KTailctl.Tailscale.status.peers
        filters: [
            ValueFilter {
                roleName: "online"
                value: page.filterOnlineValue
                enabled: page.filterOnlineEnabled
            },
            ValueFilter {
                roleName: "mullvadNode"
                value: page.filterMullvadValue
                enabled: page.filterMullvadEnabled
            }
        ]
    }

    ListView {
        anchors.fill: parent
        model: peerModel

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
                    ToolTip.text: "View node info"
                    ToolTip.visible: hovered
                    icon.name: "help-info"
                    onClicked: applicationWindow().pageStack.layers.push(pagePeerInfo, {
                        peer: KTailctl.Tailscale.status.peerWithId(id)
                    })
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
                            icon.name: "help-info"
                            text: "Node info"
                            onClicked: applicationWindow().pageStack.layers.push(pagePeerInfo, {
                                peer: KTailctl.Tailscale.status.peerWithId(id)
                            })
                        }
                    }
                }

                Item {
                    width: Kirigami.Units.largeSpacing
                }
            }
        }
    }
}
