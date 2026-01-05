import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl as KTailctl
import QtQuick

FormCard.FormCardPage {
    id: page
    property KTailctl.PeerStatus peer: KTailctl.Tailscale.status.self

    FormCard.FormHeader {
        title: "Identity"
    }

    FormCard.FormCard {
        FormCard.FormTextDelegate {
            id: peerId
            text: peer?.id ?? ""
        }

        FormCard.FormDelegateSeparator {
            below: peerId
            above: publicKey
        }

        FormCard.FormTextDelegate {
            id: publicKey
            text: peer?.publicKey ?? ""
        }

        FormCard.FormDelegateSeparator {
            below: publicKey
            above: dnsName
        }

        FormCard.FormTextDelegate {
            id: dnsName
            text: peer?.dnsName ?? ""
        }

        FormCard.FormDelegateSeparator {
            below: dnsName
            above: hostName
        }

        FormCard.FormTextDelegate {
            id: hostName
            text: peer?.hostName ?? ""
        }

        FormCard.FormDelegateSeparator {
            visible: peer?.os
            below: hostName
            above: os
        }

        FormCard.FormTextDelegate {
            visible: peer?.os
            id: os
            text: peer?.os ?? ""
        }
    }

    FormCard.FormHeader {
        title: "Addresses"
    }

    FormCard.FormCard {
        Repeater {
            model: page.peer.tailscaleIps

            FormCard.FormTextDelegate {
                text: modelData
            }
        }
    }
}