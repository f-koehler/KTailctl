import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigami as Kirigami
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/ui/components"

FormCard.FormCardPage {
    id: page
    property KTailctl.PeerStatus peer: KTailctl.Tailscale.status.self

    title: peer?.hostName ?? ""

    // Whether this page shows our own node; pinging your own Tailscale IP is a
    // local no-op, so the ping section is hidden in that case.
    readonly property bool isSelf: (peer && KTailctl.Tailscale.status.self) ? (peer.id === KTailctl.Tailscale.status.self.id) : false
    // First Tailscale IP of the peer; the target for pinging.
    readonly property string pingAddress: (peer?.tailscaleIps?.length ?? 0) > 0 ? peer.tailscaleIps[0] : ""
    readonly property bool pingAvailable: !isSelf && pingAddress !== ""
    // Lazily created, cached per-address Pinger (see Tailscale::pinger).
    readonly property var pinger: pingAvailable ? KTailctl.Tailscale.pinger(pingAddress) : null

    signal closeRequested

    actions: [
        Kirigami.Action {
            icon.name: "dialog-close"
            text: "Close"
            onTriggered: page.closeRequested()
        }
    ]

    DaemonError {}

    OperatorWarning {}

    AuthError {}

    FormCard.FormHeader {
        title: "Addresses"
    }

    FormCard.FormCard {
        Repeater {
            id: repeaterIps
            model: page.peer.tailscaleIps

            ColumnLayout {
                FormCard.FormButtonDelegate {
                    text: modelData
                    trailingLogo.source: "edit-copy"
                    onClicked: {
                        KTailctl.Util.setClipboardText(modelData);
                    }
                }
            }
        }
    }

    FormCard.FormHeader {
        visible: page.pingAvailable
        title: "Ping"
    }

    FormCard.FormCard {
        visible: page.pingAvailable

        FormCard.AbstractFormDelegate {
            background: null
            contentItem: ColumnLayout {
                spacing: Kirigami.Units.largeSpacing

                RowLayout {
                    Layout.fillWidth: true
                    spacing: Kirigami.Units.largeSpacing

                    Button {
                        icon.name: (page.pinger && page.pinger.running) ? "media-playback-stop" : "media-playback-start"
                        text: (page.pinger && page.pinger.running) ? "Stop" : "Start"
                        onClicked: {
                            if (page.pinger) {
                                page.pinger.toggle();
                            }
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                        elide: Text.ElideRight
                        color: (page.pinger && page.pinger.hasResult && !page.pinger.lastSuccess) ? Kirigami.Theme.negativeTextColor : Kirigami.Theme.textColor
                        text: {
                            const p = page.pinger;
                            if (!p) {
                                return "";
                            }
                            KTailctl.Tailscale.lastRefresh;
                            if (!p.hasResult) {
                                return p.running ? "Pinging…" : "Not pinged yet";
                            }
                            const when = KTailctl.Util.formatDurationHumanReadable(p.lastTime);
                            if (p.lastSuccess) {
                                return p.lastLatencyMs.toFixed(1) + " ms via " + p.lastVia + " · " + when;
                            }
                            return (p.lastError !== "" ? p.lastError : "no reply") + " · " + when;
                        }
                    }
                }

                PingGraph {
                    Layout.fillWidth: true
                    implicitHeight: Kirigami.Units.gridUnit * 5
                    pinger: page.pinger
                    visible: page.pinger && (page.pinger.running || page.pinger.hasResult)
                }
            }
        }
    }

    FormCard.FormHeader {
        visible: (peer?.tags?.length ?? 0) > 0
        title: "Tags"
    }

    FormCard.FormCard {

        visible: (peer?.tags?.length ?? 0) > 0

        FormCard.AbstractFormDelegate {
            background: null
            contentItem: Flow {
                spacing: Kirigami.Units.smallSpacing
                Repeater {
                    model: peer?.tags ?? []
                    Kirigami.Chip {
                        text: modelData
                        closable: false
                        onClicked: KTailctl.Util.setClipboardText(modelData)
                        checkable: false
                    }
                }
            }
        }
    }

    FormCard.FormHeader {
        title: "Identity"
    }

    FormCard.FormCard {
        FormCard.FormButtonDelegate {
            id: peerId
            text: peer?.id ?? ""
            description: "Node ID"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.id);
            }
        }

        FormCard.FormButtonDelegate {
            id: publicKey
            text: peer?.publicKey ?? ""
            description: "Public Key"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.publicKey);
            }
        }

        FormCard.FormButtonDelegate {
            id: dnsName
            text: peer?.dnsName ?? ""
            description: "DNS Name"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.dnsName);
            }
        }

        FormCard.FormButtonDelegate {
            id: hostName
            text: peer?.hostName ?? ""
            description: "Host Name"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.hostName);
            }
        }

        FormCard.FormButtonDelegate {
            id: os
            visible: peer?.os
            text: peer?.os ?? ""
            description: "Operating System"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.os);
            }
        }

        FormCard.FormButtonDelegate {
            id: created
            visible: peer?.created && !isNaN(peer.created)
            text: KTailctl.Util.formatDurationHumanReadable(peer?.created)
            description: "Created"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.created.toISOString());
            }
        }

        FormCard.FormButtonDelegate {
            id: lastSeen
            visible: peer?.lastSeen && !isNaN(peer.lastSeen)
            text: KTailctl.Util.formatDurationHumanReadable(peer?.lastSeen)
            description: "Last Seen"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.lastSeen.toISOString());
            }
        }

        FormCard.FormLinkDelegate {
            id: adminPanelUrl
            url: (peer?.tailscaleIps?.length ?? 0) > 0 ? "https://login.tailscale.com/admin/machines/" + peer.tailscaleIps[0] : "https://login.tailscale.com/admin/machines/"
            text: adminPanelUrl.url
            description: "Admin Panel"
        }
    }

    FormCard.FormHeader {
        title: "Primary Routes"
    }

    FormCard.FormCard {
        visible: peer.primaryRoutes.length == 0
        FormCard.FormTextDelegate {
            text: "None"
        }
    }
    FormCard.FormCard {
        visible: peer.primaryRoutes.length > 0
        Repeater {
            id: repeaterRoutes
            model: peer.primaryRoutes

            ColumnLayout {
                FormCard.FormButtonDelegate {
                    text: modelData
                    trailingLogo.source: "edit-copy"
                    onClicked: {
                        KTailctl.Util.setClipboardText(modelData);
                    }
                }
            }
        }
    }

    FormCard.FormHeader {
        visible: peer.location !== null
        title: "Location"
    }

    FormCard.FormCard {
        visible: peer.location !== null
        FormCard.FormButtonDelegate {
            id: country
            text: peer.location?.country + " (" + peer.location?.countryCode + ")"
            description: "Country (Country Code)"
            leading: Kirigami.Icon {
                source: "qrc:/country-flags/country-flag-" + peer.location?.countryCode.toLowerCase()
            }
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.location.country);
            }
        }

        FormCard.FormButtonDelegate {
            id: city
            text: peer.location?.city + " (" + peer.location?.cityCode + ")"
            description: "City (City Code)"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.location.city);
            }
        }

        FormCard.FormButtonDelegate {
            id: latitude
            text: peer?.location?.latitude
            description: "Latitude"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.location.latitude);
            }
        }

        FormCard.FormButtonDelegate {
            id: longitude
            text: peer?.location?.longitude
            description: "Longitude"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.location.longitude);
            }
        }

        FormCard.FormButtonDelegate {
            id: priority
            text: peer?.location?.priority
            description: "Priority"
            trailingLogo.source: "edit-copy"
            onClicked: {
                KTailctl.Util.setClipboardText(peer.location.priority);
            }
        }
    }
}
