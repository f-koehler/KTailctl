pragma ComponentBehavior: Bound

import QtQuick.Controls as QQC2
import QtQuick.Layouts
import QtQuick
import QtQml.Models
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import "qrc:/ui/components"

Kirigami.ScrollablePage {
    id: page

    title: "Peers"

    Layout.fillWidth: true

    property bool filterMullvadEnabled: true
    property bool filterMullvadValue: false
    property bool filterOnlineEnabled: false
    property bool filterOnlineValue: true
    property bool filterDnsNameEnabled: false
    property string filterDnsNameValue: ""

    // Id of the peer currently shown in the detail column (empty == none).
    property string selectedId: ""

    function deviceIcon(os: string): string {
        const o = (os ?? "").toLowerCase();
        return (o === "android" || o === "ios") ? "smartphone" : "computer";
    }

    // Close the detail column and clear the selection in the list.
    function closeDetail(): void {
        applicationWindow().pageStack.pop(page);
        page.selectedId = "";
    }

    // Open (or replace) the detail column for the given peer.
    function showPeer(peerId: string): void {
        const stack = applicationWindow().pageStack;
        page.selectedId = peerId;
        // Drop any existing detail column so we always have exactly one.
        stack.pop(page);
        const detail = stack.push(peerInfoComponent, {
            peer: KTailctl.Tailscale.status.peerWithId(peerId)
        });
        detail.closeRequested.connect(page.closeDetail);
    }

    Component {
        id: peerInfoComponent
        PeerInfo {}
    }

    // A single row: device icon + host name on the left, online dot on the right.
    component PeerRow: QQC2.ItemDelegate {
        id: row

        property string peerId: ""
        property string hostLabel: ""
        property bool isOnline: false
        property string osName: ""

        highlighted: page.selectedId === peerId
        onClicked: page.showPeer(peerId)

        contentItem: RowLayout {
            spacing: Kirigami.Units.largeSpacing

            Kirigami.Icon {
                source: page.deviceIcon(row.osName)
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
            }

            QQC2.Label {
                Layout.fillWidth: true
                text: row.hostLabel
                elide: Text.ElideRight
            }

            Kirigami.Icon {
                source: row.isOnline ? "online" : "offline"
                implicitWidth: Kirigami.Units.iconSizes.small
                implicitHeight: Kirigami.Units.iconSizes.small

                HoverHandler {
                    id: hoverHandler
                }

                QQC2.ToolTip {
                    visible: hoverHandler.hovered
                    delay: Kirigami.Units.toolTipDelay
                    text: row.isOnline ? "Online" : "Offline"
                }
            }
        }
    }

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
            id: actionSearch

            icon.name: "search"
            text: "DNS Name"

            displayComponent: Kirigami.SearchField {
                id: searchField
                onAccepted: {
                    page.filterDnsNameValue = searchField.text;
                    page.filterDnsNameEnabled = searchField.text.length > 0;
                }
            }
        },
        Kirigami.Action {
            id: actionFilter
            icon.name: "dialog-filters"

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
    ]

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
        sorters: RoleSorter {
            roleName: "hostName"
        }
    }

    KTailctl.StringFilter {
        id: dnsNameFilter
        sourceModel: peerModel
        filterRoleNames: ["dnsName"]
        filterString: page.filterDnsNameEnabled ? page.filterDnsNameValue : ""
    }

    ListView {
        id: peerListView

        model: dnsNameFilter

        header: ColumnLayout {
            width: peerListView.width
            spacing: 0

            DaemonError {}
            OperatorWarning {}
            AuthError {}

            Kirigami.ListSectionHeader {
                Layout.fillWidth: true
                text: "This Node"
            }

            PeerRow {
                Layout.fillWidth: true
                peerId: KTailctl.Tailscale.status.self.id
                hostLabel: KTailctl.Tailscale.status.self.hostName
                isOnline: KTailctl.Tailscale.status.self.online
                osName: KTailctl.Tailscale.status.self.os
            }

            Kirigami.ListSectionHeader {
                Layout.fillWidth: true
                text: "Peers"
            }
        }

        delegate: PeerRow {
            id: delegate

            required property string id
            required property bool online
            required property string hostName
            required property string os

            width: ListView.view.width
            peerId: id
            hostLabel: hostName
            isOnline: online
            osName: os
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - Kirigami.Units.gridUnit * 4
            visible: peerListView.count === 0
            icon.name: "distribute-graph-directed"
            text: "No peers match the current filters"
        }
    }
}
