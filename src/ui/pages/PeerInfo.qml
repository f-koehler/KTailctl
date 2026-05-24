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

    DaemonError {}

    OperatorWarning {}

    AuthError {}

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
                    }
                }
            }
        }
    }

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
