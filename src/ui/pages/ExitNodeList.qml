import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigamiaddons.formcard as FormCard
import "qrc:/ui/components"

FormCard.FormCardPage {
    id: page

    Layout.fillWidth: true

    readonly property bool isOperator: {
        const op = KTailctl.Tailscale.preferences.operatorUser;
        return op !== "" && op === KTailctl.Util.systemUser();
    }

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
                    if (page.filterMullvadSearchValue) {
                        // we need to toggle the filter to refresh it, just updating the filter value is not sufficient
                        page.filterMullvadSearchEnabled = false;
                        page.filterMullvadSearchEnabled = true;
                    } else {
                        page.filterMullvadSearchEnabled = false;
                    }
                }
            }
        }
    ]

    DaemonError {}

    OperatorWarning {}

    AuthError {}

    FormCard.FormHeader {
        title: "Settings"
    }

    FormCard.FormCard {
        // FormCard.FormSwitchDelegate {
        //     text: "Advertise exit node"
        //     value:
        // }
        FormCard.FormSwitchDelegate {
            id: switchAllowLanAccess
            enabled: page.isOperator
            text: "Allow LAN access"
            checked: KTailctl.Tailscale.preferences.exitNodeAllowLanAccess
            onCheckedChanged: KTailctl.Tailscale.preferences.exitNodeAllowLanAccess = switchAllowLanAccess.checked
        }

        FormCard.FormSwitchDelegate {
            id: switchExitNode

            readonly property string currentId: KTailctl.Tailscale.preferences.exitNodeId
            readonly property string lastUsedId: KTailctl.Config.lastUsedExitNode

            enabled: page.isOperator
            // When an exit node is active, show it. Otherwise fall back to the
            // last used one; hide entirely if there is neither.
            visible: currentId !== "" || lastUsedId !== ""
            text: currentId !== "" ? (KTailctl.Tailscale.status.peerWithId(currentId)?.dnsName ?? currentId) : (KTailctl.Tailscale.status.peerWithId(lastUsedId)?.dnsName ?? lastUsedId)
            description: currentId !== "" ? "Current exit node" : "Last used exit node"
            onToggled: {
                // `checked` has already been flipped by the control.
                KTailctl.Tailscale.preferences.exitNodeId = checked ? switchExitNode.lastUsedId : "";
            }
        }

        // Keep the switch in sync with the active exit node even when it is
        // changed elsewhere (other delegates, tray, another client).
        Binding {
            target: switchExitNode
            property: "checked"
            value: KTailctl.Tailscale.preferences.exitNodeId !== ""
        }

        FormCard.FormDelegateSeparator {
            visible: KTailctl.Tailscale.status.suggestedExitNodeId !== "" && KTailctl.Tailscale.status.suggestedExitNodeId !== KTailctl.Tailscale.preferences.exitNodeId
        }

        FormCard.FormButtonDelegate {
            id: suggestedDelegate

            property var suggestedPeer: KTailctl.Tailscale.status.suggestedExitNodeId !== "" ? KTailctl.Tailscale.status.peerWithId(KTailctl.Tailscale.status.suggestedExitNodeId) : null

            enabled: page.isOperator
            visible: KTailctl.Tailscale.status.suggestedExitNodeId !== "" && KTailctl.Tailscale.status.suggestedExitNodeId !== KTailctl.Tailscale.preferences.exitNodeId
            text: "Use suggested: " + (suggestedPeer?.dnsName ?? KTailctl.Tailscale.status.suggestedExitNodeId)
            leading: Kirigami.Icon {
                source: suggestedDelegate.suggestedPeer?.mullvadNode && suggestedDelegate.suggestedPeer?.location ? "qrc:/country-flags/country-flag-" + suggestedDelegate.suggestedPeer.location.countryCode.toLowerCase() : "network-vpn"
            }
            onClicked: {
                KTailctl.Tailscale.preferences.exitNodeId = KTailctl.Tailscale.status.suggestedExitNodeId;
            }
        }
    }

    FormCard.FormHeader {
        title: "Self-Hosted Exit Nodes"
    }

    FormCard.FormCard {
        visible: KTailctl.Tailscale.status.selfHostedExitNodeModel.rowCount() === 0

        FormCard.FormTextDelegate {
            text: "No self-hosted exit nodes available"
        }
    }

    FormCard.FormCard {
        visible: KTailctl.Tailscale.status.selfHostedExitNodeModel.rowCount() > 0

        Repeater {
            model: KTailctl.Tailscale.status.selfHostedExitNodeModel
            delegate: FormCard.FormTextDelegate {
                text: dnsName
                // description: mullvadNode ? "Mullvad Exit Node" : "Self-Hosted Exit Node"
                leading: Kirigami.Icon {
                    ToolTip.delay: Kirigami.Units.toolTipDelay
                    ToolTip.text: "Country"
                    ToolTip.visible: hovered
                    source: "network-vpn"
                }
                trailing: RowLayout {
                    ToolButton {
                        enabled: page.isOperator
                        ToolTip.delay: Kirigami.Units.toolTipDelay
                        ToolTip.text: "Enable exit node"
                        ToolTip.visible: hovered
                        icon.name: "system-switch-user"
                        onClicked: {
                            KTailctl.Tailscale.preferences.exitNodeId = id;
                        }
                    }

                    ToolButton {
                        ToolTip.delay: Kirigami.Units.toolTipDelay
                        ToolTip.text: "View node info"
                        ToolTip.visible: hovered
                        icon.name: "info"
                        onClicked: applicationWindow().pageStack.layers.push(pagePeerInfo, {
                            peer: KTailctl.Tailscale.status.peerWithId(id)
                        })
                    }
                }
            }
        }
    }

    FormCard.FormHeader {
        title: "Mullvad Exit Nodes"
    }

    FormCard.FormCard {
        visible: KTailctl.Tailscale.status.mullvadExitNodeModel.rowCount() === 0

        FormCard.FormTextDelegate {
            text: "No Mullvad exit nodes available"
        }
    }

    Loader {
        active: KTailctl.Tailscale.status.mullvadExitNodeModel.rowCount() > 0
        asynchronous: true

        Layout.fillWidth: true

        sourceComponent: FormCard.FormCard {
            Repeater {
                model: KTailctl.Tailscale.status.mullvadExitNodeModel
                delegate: FormCard.FormTextDelegate {
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
                            enabled: page.isOperator
                            ToolTip.delay: Kirigami.Units.toolTipDelay
                            ToolTip.text: "Enable exit node"
                            ToolTip.visible: hovered
                            icon.name: "system-switch-user"
                            onClicked: {
                                KTailctl.Tailscale.preferences.exitNodeId = id;
                            }
                        }

                        ToolButton {
                            ToolTip.delay: Kirigami.Units.toolTipDelay
                            ToolTip.text: "View node info"
                            ToolTip.visible: hovered
                            icon.name: "info"
                            onClicked: applicationWindow().pageStack.layers.push(pagePeerInfo, {
                                peer: KTailctl.Tailscale.status.peerWithId(id)
                            })
                        }
                    }
                }
            }
        }
    }
}
