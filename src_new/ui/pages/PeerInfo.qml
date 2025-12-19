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
            text: page.peer.id
        }

        FormCard.FormDelegateSeparator {
            below: peerId
            above: publicKey
        }

        FormCard.FormTextDelegate {
            id: publicKey
            text: page.peer.publicKey
        }

        FormCard.FormDelegateSeparator {
            below: publicKey
            above: dnsName
        }

        FormCard.FormTextDelegate {
            id: dnsName
            text: page.peer.dnsName
        }

        FormCard.FormDelegateSeparator {
            below: dnsName
            above: hostName
        }

        FormCard.FormTextDelegate {
            id: hostName
            text: page.peer.hostName
        }

        FormCard.FormDelegateSeparator {
            below: hostName
            above: os
        }

        FormCard.FormTextDelegate {
            id: os
            text: page.peer.os
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