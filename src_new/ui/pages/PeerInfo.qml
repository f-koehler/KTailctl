import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigami as Kirigami
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

FormCard.FormCardPage {
    id: page
    property KTailctl.PeerStatus peer: KTailctl.Tailscale.status.self

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
                onClicked: KTailctl.Util.setClipboardText(peer.id);
            }
        }

        FormCard.FormButtonDelegate {
            id: publicKey
            text: peer?.publicKey ?? ""
            description: "Public Key"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.publicKey);
            }
        }

        FormCard.FormButtonDelegate {
            id: dnsName
            text: peer?.dnsName ?? ""
            description: "DNS Name"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.dnsName);
            }
        }

        FormCard.FormButtonDelegate {
            id: hostName
            text: peer?.hostName ?? ""
            description: "Host Name"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.hostName);
            }
        }

        FormCard.FormButtonDelegate {
            id: os
            visible: peer?.os
            text: peer?.os ?? ""
            description: "Operating System"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.os);
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
                        onClicked: KTailctl.Util.setClipboardText(modelData);
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
                        onClicked: KTailctl.Util.setClipboardText(modelData);
                    }
                }
            }
        }
    }

    FormCard.FormHeader {
        title: "Location"
    }

    FormCard.FormCard {
        FormCard.FormButtonDelegate {
            id: country
            text: peer.location?.country + " (" + peer.location?.countryCode + ")"
            description: "Country (Country Code)"
            leading: Kirigami.Icon {
                source: "flag"
            }
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.country);
            }
        }

        FormCard.FormButtonDelegate {
            id: city
            text: peer.location?.city + " (" + peer.location?.cityCode + ")"
            description: "City (City Code)"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.city);
            }
        }

        FormCard.FormButtonDelegate {
            id: latitude
            text: peer?.location?.latitude
            description: "Latitude"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.latitude);
            }
        }

        FormCard.FormButtonDelegate {
            id: longitude
            text: peer?.location?.longitude
            description: "Longitude"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.longitude);
            }
        }

        FormCard.FormButtonDelegate {
            id: priority
            text: peer?.location?.priority
            description: "Priority"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.priority);
            }
        }
    }
}
