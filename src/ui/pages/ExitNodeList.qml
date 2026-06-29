pragma ComponentBehavior: Bound

import QtQuick.Controls as QQC2
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigamiaddons.formcard as FormCard
import "qrc:/ui/components"

Kirigami.ScrollablePage {
    id: page

    title: "Exit Nodes"

    Layout.fillWidth: true

    readonly property bool isOperator: {
        const op = KTailctl.Tailscale.preferences.operatorUser;
        return op !== "" && op === KTailctl.Util.systemUser();
    }

    // Id of the node shown in the detail column (empty == none).
    property string selectedId: ""
    property bool filterDnsNameEnabled: false
    property string filterDnsNameValue: ""

    // Close the detail column and clear the selection in the list.
    function closeDetail(): void {
        applicationWindow().pageStack.pop(page);
        page.selectedId = "";
    }

    // Open (or replace) the detail column for the given node.
    function showNode(nodeId: string): void {
        const stack = applicationWindow().pageStack;
        page.selectedId = nodeId;
        // Drop any existing detail column so we always have exactly one.
        stack.pop(page);
        const detail = stack.push(peerInfoComponent, {
            peer: KTailctl.Tailscale.status.peerWithId(nodeId)
        });
        detail.closeRequested.connect(page.closeDetail);
    }

    Component {
        id: peerInfoComponent
        PeerInfo {}
    }

    // A single exit-node row: icon + dns name on the left, a switch on the
    // right that reflects/sets whether this node is the current exit node.
    // Clicking the row opens the detail column.
    component ExitNodeRow: QQC2.ItemDelegate {
        id: row

        property string nodeId: ""
        property string label: ""
        property string sublabel: ""
        property string iconSource: "network-vpn"

        highlighted: page.selectedId === nodeId
        onClicked: page.showNode(nodeId)

        contentItem: RowLayout {
            spacing: Kirigami.Units.largeSpacing

            Kirigami.Icon {
                source: row.iconSource
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
            }

            QQC2.Label {
                text: row.label
                elide: Text.ElideRight
                Layout.maximumWidth: Kirigami.Units.gridUnit * 18
            }

            // Location sublabel: fills remaining space and elides when too long.
            QQC2.Label {
                visible: row.sublabel !== ""
                text: row.sublabel
                elide: Text.ElideRight
                opacity: 0.7
                Layout.fillWidth: true
            }

            // Spacer when there is no sublabel, to keep the switch right-aligned.
            Item {
                visible: row.sublabel === ""
                Layout.fillWidth: true
            }

            QQC2.Switch {
                id: enableSwitch
                enabled: page.isOperator

                QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
                QQC2.ToolTip.text: checked ? "Disable exit node" : "Enable exit node"
                QQC2.ToolTip.visible: hovered

                onToggled: {
                    // `checked` has already been flipped by the control.
                    KTailctl.Tailscale.preferences.exitNodeId = checked ? row.nodeId : "";
                }

                // Keep in sync with the active exit node even when it changes
                // elsewhere (other rows, the settings switch, tray, ...).
                Binding {
                    target: enableSwitch
                    property: "checked"
                    value: KTailctl.Tailscale.preferences.exitNodeId === row.nodeId
                }
            }
        }
    }

    actions: [
        Kirigami.Action {
            id: actionSearch

            icon.name: "search"
            text: "Search"

            displayComponent: Kirigami.SearchField {
                id: searchField
                onAccepted: {
                    page.filterDnsNameValue = searchField.text;
                    page.filterDnsNameEnabled = searchField.text.length > 0;
                }
            }
        }
    ]

    KTailctl.StringFilter {
        id: mullvadFilter
        sourceModel: KTailctl.Tailscale.status.mullvadExitNodeModel
        // Match the DNS name as well as the location's city/country names, so a
        // node can be found without knowing its city/country code.
        filterRoleNames: ["dnsName", "location"]
        filterString: page.filterDnsNameEnabled ? page.filterDnsNameValue : ""
    }

    ListView {
        id: nodeListView

        model: mullvadFilter

        header: ColumnLayout {
            width: nodeListView.width
            spacing: 0

            DaemonError {}
            OperatorWarning {}
            AuthError {}

            FormCard.FormHeader {
                Layout.fillWidth: true
                title: "Settings"
            }

            FormCard.FormCard {
                Layout.fillWidth: true

                FormCard.FormSwitchDelegate {
                    id: switchExitNode

                    readonly property string currentId: KTailctl.Tailscale.preferences.exitNodeId
                    readonly property string lastUsedId: KTailctl.Config.lastUsedExitNode

                    enabled: page.isOperator
                    // When an exit node is active, show it. Otherwise fall back
                    // to the last used one; hide entirely if there is neither.
                    visible: currentId !== "" || lastUsedId !== ""
                    text: currentId !== "" ? (KTailctl.Tailscale.status.peerWithId(currentId)?.dnsName ?? currentId) : (KTailctl.Tailscale.status.peerWithId(lastUsedId)?.dnsName ?? lastUsedId)
                    description: currentId !== "" ? "Current exit node" : "Last used exit node"
                    onToggled: {
                        // `checked` has already been flipped by the control.
                        KTailctl.Tailscale.preferences.exitNodeId = checked ? switchExitNode.lastUsedId : "";
                    }
                }

                // Keep the switch in sync with the active exit node even when it
                // is changed elsewhere (rows, tray, another client).
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

            Kirigami.ListSectionHeader {
                Layout.fillWidth: true
                text: "Self-Hosted Exit Nodes"
            }

            QQC2.Label {
                Layout.fillWidth: true
                Layout.margins: Kirigami.Units.largeSpacing
                visible: selfHostedRepeater.count === 0
                horizontalAlignment: Text.AlignHCenter
                opacity: 0.7
                text: "No self-hosted exit nodes available"
            }

            Repeater {
                id: selfHostedRepeater
                model: KTailctl.Tailscale.status.selfHostedExitNodeModel
                delegate: ExitNodeRow {
                    required property string id
                    required property string hostName

                    Layout.fillWidth: true
                    nodeId: id
                    label: hostName
                    iconSource: "network-vpn"
                }
            }

            Kirigami.ListSectionHeader {
                Layout.fillWidth: true
                text: "Mullvad Exit Nodes"
            }

            QQC2.Label {
                Layout.fillWidth: true
                Layout.margins: Kirigami.Units.largeSpacing
                visible: nodeListView.count === 0
                horizontalAlignment: Text.AlignHCenter
                opacity: 0.7
                text: page.filterDnsNameEnabled ? "No exit nodes match the search" : "No Mullvad exit nodes available"
            }
        }

        delegate: ExitNodeRow {
            id: delegate

            required property string id
            required property string hostName
            required property var location

            width: ListView.view.width
            nodeId: id
            label: hostName
            sublabel: location ? "(" + location.city + ", " + location.country + ")" : ""
            iconSource: location ? "qrc:/country-flags/country-flag-" + location.countryCode.toLowerCase() : "network-vpn"
        }
    }
}
