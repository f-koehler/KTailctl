import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl as KTailctl
import QtQuick
import QtQuick.Controls

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
            description: "Node ID"
            trailing: ToolButton {
                visible: peer?.id
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(peer.id)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: peerId
            above: publicKey
        }

        FormCard.FormTextDelegate {
            id: publicKey
            text: peer?.publicKey ?? ""
            description: "Public Key"
            trailing: ToolButton {
                visible: peer?.publicKey
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(peer.publicKey)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: publicKey
            above: dnsName
        }

        FormCard.FormTextDelegate {
            id: dnsName
            text: peer?.dnsName ?? ""
            description: "DNS Name"
            trailing: ToolButton {
                visible: peer?.dnsName
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(peer.dnsName)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: dnsName
            above: hostName
        }

        FormCard.FormTextDelegate {
            id: hostName
            text: peer?.hostName ?? ""
            description: "Host Name"
            trailing: ToolButton {
                visible: peer?.hostName
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(peer.hostName)
                }
            }
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
            description: "Operating System"
            trailing: ToolButton {
                visible: peer?.os
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(peer.os)
                }
            }
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
                trailing: ToolButton {
                    icon.name: "edit-copy"
                    onClicked: {
                        onClicked: KTailctl.Util.setClipboardText(modelData)
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
            model: peer.primaryRoutes

            FormCard.FormTextDelegate {
                text: modelData

                FormCard.FormTextDelegate {
                    text: modelData
                    trailing: ToolButton {
                        icon.name: "edit-copy"
                        onClicked: {
                            onClicked: KTailctl.Util.setClipboardText(modelData)
                        }
                    }
                }
            }
        }
    }
}