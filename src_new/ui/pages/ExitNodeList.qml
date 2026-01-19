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

    property bool filterMullvadSearchEnabled: false
    property string filterMullvadSearchValue: ""

    actions: [
        Kirigami.Action {
            id: actionSearch

            icon.name: "search"
            text: "Search"

            displayComponent: Kirigami.SearchField {
                id: searchField
                onAccepted: {
                    page.filterMullvadSearchValue = searchField.text.toLowerCase();
                    if(page.filterMullvadSearchValue) {
                        // we need to toggle the filter to refresh it, just updating the filter value is not sufficient
                        page.filterMullvadSearchEnabled = false;
                        page.filterMullvadSearchEnabled = true;
                    } else {
                        page.filterMullvadSearchEnabled = false;
                    }
                }
            }
        },
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
            Models.FunctionFilter {
                id: functionFilterDnsName
                enabled: page.filterMullvadSearchEnabled
                component RoleData: QtObject {
                    property string dnsName
                    property KTailctl.Location location
                }
                function filter(data: RoleData) : bool {
                    if(data.location) {
                        if(data.location.city.toLowerCase().includes(page.filterMullvadSearchValue)) {
                            return true;
                        }
                        if(data.location.country.toLowerCase().includes(page.filterMullvadSearchValue)) {
                            return true;
                        }
                    }
                    return data.dnsName.includes(page.filterMullvadSearchValue)
                }
            }
        ]
        sorters: [
            Models.StringSorter { roleName: "dnsName" }
        ]
    }

    Models.SortFilterProxyModel {
        id: selfHostedExitNodeModel
        model: exitNodeModel
        filters: [
            Models.ValueFilter {
                roleName: "mullvadNode"
                value: false
            }
        ]
    }

    Models.SortFilterProxyModel {
        id: mullvadExitNodeModel
        model: exitNodeModel
        filters: [
            Models.ValueFilter {
                roleName: "mullvadNode"
                value: true
            }
        ]
    }

    FormCard.FormHeader {
           title: "Settings"
    }

    FormCard.FormCard {
        // FormCard.FormSwitchDelegate {
        //     text: "Advertise exit node"
        //     value:
        // }
        FormCard.FormSwitchDelegate {
            text: "Allow LAN access"
            checked: KTailctl.Tailscale.preferences.exitNodeAllowLanAccess
        }

        FormCard.FormTextDelegate {
            text: "Current exit node: None"
        }
    }

    FormCard.FormHeader {
        title: "Self-Hosted Exit Nodes"
    }

    FormCard.FormCard {
        Repeater {
            model: selfHostedExitNodeModel
            delegate: ColumnLayout {
                FormCard.FormTextDelegate {
                    text: dnsName
                    // description: mullvadNode ? "Mullvad Exit Node" : "Self-Hosted Exit Node"
                    leading: Kirigami.Icon {
                        ToolTip.delay: Kirigami.Units.toolTipDelay
                        ToolTip.text: "Country"
                        ToolTip.visible: hovered
                        source:  "network-vpn"
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

    FormCard.FormHeader {
        title: "Mullvad Exit Nodes"
    }

    FormCard.FormCard {
        Repeater {
            model: mullvadExitNodeModel
            delegate: ColumnLayout {
                FormCard.FormTextDelegate {
                    text: dnsName
                    description: location.city + " (" + location.country + ")"
                    leading: Kirigami.Icon {
                        ToolTip.delay: Kirigami.Units.toolTipDelay
                        ToolTip.text: location.city + " (" + location.country + ")"
                        ToolTip.visible: hovered
                        source: "qrc:/country-flags/country-flag-" + location.countryCode.toLowerCase()
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
