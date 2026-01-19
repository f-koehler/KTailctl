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
                onClicked: KTailctl.Util.setClipboardText(peer.id)
            }
        }

        FormCard.FormDelegateSeparator {
            below: peerId
            above: publicKey
        }

        FormCard.FormButtonDelegate {
            id: publicKey
            text: peer?.publicKey ?? ""
            description: "Public Key"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.publicKey)
            }
        }

        FormCard.FormDelegateSeparator {
            below: publicKey
            above: dnsName
        }

        FormCard.FormButtonDelegate {
            id: dnsName
            text: peer?.dnsName ?? ""
            description: "DNS Name"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.dnsName)
            }
        }

        FormCard.FormDelegateSeparator {
            below: dnsName
            above: hostName
        }

        FormCard.FormButtonDelegate {
            id: hostName
            text: peer?.hostName ?? ""
            description: "Host Name"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.hostName)
            }
        }

        FormCard.FormDelegateSeparator {
            visible: peer?.os
            below: hostName
            above: os
        }

        FormCard.FormButtonDelegate {
            visible: peer?.os
            id: os
            text: peer?.os ?? ""
            description: "Operating System"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.os)
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
                        onClicked: KTailctl.Util.setClipboardText(modelData)
                    }
                }
                FormCard.FormDelegateSeparator {
                    visible: index < repeaterIps.count - 1
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
                        onClicked: KTailctl.Util.setClipboardText(modelData)
                    }
                    FormCard.FormDelegateSeparator {
                        visible: index < repeaterRoutes.count - 1
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
                onClicked: KTailctl.Util.setClipboardText(peer.location.country)
            }
        }

        FormCard.FormDelegateSeparator {
            below: country
            above: city
        }

        FormCard.FormButtonDelegate {
            id: city
            text: peer.location?.city + " (" + peer.location?.cityCode + ")"
            description: "City (City Code)"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.city)
            }
        }

        FormCard.FormDelegateSeparator {
            below: city
            above: latitude
        }

        FormCard.FormButtonDelegate {
            id: latitude
            text: peer?.location?.latitude
            description: "Latitude"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.latitude)
            }
        }

        FormCard.FormDelegateSeparator {
            below: latitude
            above: longitude
        }

        FormCard.FormButtonDelegate {
            id: longitude
            text: peer?.location?.longitude
            description: "Longitude"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.longitude)
            }
        }

        FormCard.FormDelegateSeparator {
            below: longitude
            above: priority
        }

        FormCard.FormButtonDelegate {
            id: priority
            text: peer?.location?.priority
            description: "Priority"
            trailingLogo.source: "edit-copy"
            onClicked: {
                onClicked: KTailctl.Util.setClipboardText(peer.location.priority)
            }
        }
    }
}