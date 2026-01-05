import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import QtQml.Models as Models
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    id: page

    Layout.fillWidth: true

    Component {
        id: pagePeerInfo

        PeerInfo {}
    }

    property bool filterMullvadEnabled: false
    property bool filterMullvadValue: true

    actions: [
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
                text: "Mullvad"
                checkable: true
                checked: page.filterMullvadEnabled
                icon.name: "globe"
                onToggled: {
                    page.filterMullvadEnabled = !page.filterMullvadEnabled;
                }
            }
        }
    ]

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
            Models.ValueFilter {
                roleName: "mullvadNode"
                value: page.filterMullvadValue
                enabled: page.filterMullvadEnabled
            }
        ]
    }

    FormCard.FormHeader {
        title: "Current Exit Node"
    }

    FormCard.FormCard {
        FormCard.FormTextDelegate {
            text: "None"
        }
    }

    FormCard.FormHeader {
        title: "Available Exit Nodes"
    }

    // FormCard.FormCard {
    //     // visible: exitNodeModel.rowCount() == 0
    //     FormCard.FormTextDelegate {
    //         text: "None"
    //     }
    // }

    FormCard.FormCard {
        // visible: exitNodeModel.rowCount() > 0
        Repeater {
            model: exitNodeModel
            delegate: ColumnLayout {
                FormCard.FormTextDelegate {
                    text: dnsName
                    description: mullvadNode ? "Mullvad Exit Node" : "Self-Hosted Exit Node"
                    leading: Kirigami.Icon {
                        ToolTip.delay: Kirigami.Units.toolTipDelay
                        ToolTip.text: "Country"
                        ToolTip.visible: hovered
                        source: "flag"
                    }
                    trailing: RowLayout {
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
                    }
                }

                FormCard.FormDelegateSeparator {
                    visible: index < exitNodeModel.rowCount() - 1
                }
            }
        }
    }
}
