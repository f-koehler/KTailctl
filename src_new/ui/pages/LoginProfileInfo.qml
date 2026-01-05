import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl as KTailctl
import QtQuick
import QtQuick.Controls

FormCard.FormCardPage {
    property KTailctl.LoginProfile loginProfile: null

    Component.onCompleted: {
        console.log("Login profile:", loginProfile)
    }

    FormCard.FormHeader {
        title: "Login Profile"
    }

    FormCard.FormCard {
        FormCard.FormTextDelegate {
            id: loginProfileId
            text: loginProfile?.id ?? ""
            description: "ID"
            trailing: ToolButton {
                visible: loginProfile?.id
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.id)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: loginProfileId
            above: name
        }

        FormCard.FormTextDelegate {
            id: name
            text: loginProfile?.name ?? ""
            description: "Name"
            trailing: ToolButton {
                visible: loginProfile?.name
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.name)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: name
            above: key
        }

        FormCard.FormTextDelegate {
            id: key
            text: loginProfile?.key ?? ""
            description: "Key"
            trailing: ToolButton {
                visible: loginProfile?.key
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.key)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: key
            above: nodeId
        }

        FormCard.FormTextDelegate {
            id: nodeId
            text: loginProfile?.nodeId ?? ""
            description: "Node ID"
            trailing: ToolButton {
                visible: loginProfile?.nodeId
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.nodeId)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: nodeId
            above: controlUrl
        }

        // currently only set on Windows
        // FormCard.FormTextDelegate {
        //     id: localUserId
        //     text: loginProfile?.localUserId ?? ""
        // }

        FormCard.FormTextDelegate {
            id: controlUrl
            text: loginProfile?.controlUrl ?? ""
            description: "Control Pane URL"
            trailing: ToolButton {
                visible: loginProfile?.controlUrl
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.controlUrl)
                }
            }
        }
    }

    FormCard.FormHeader {
        title: "User Profile"
    }

    FormCard.FormCard {
        FormCard.FormTextDelegate {
            id: userId
            text: toString(loginProfile?.userProfile?.userId) ?? ""
            description: "User ID"
            trailing: ToolButton {
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(toString(loginProfile.userProfile.userId))
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: userId
            above: loginName
        }

        FormCard.FormTextDelegate {
            id: loginName
            description: "Login Name"
            text: loginProfile?.userProfile?.loginName ?? ""
            trailing: ToolButton {
                visible: loginProfile?.userProfile?.loginName
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.userProfile.loginName)
                }
            }
        }
    }

    FormCard.FormHeader {
        title: "Network Profile Profile"
    }

    FormCard.FormCard {
        FormCard.FormTextDelegate {
            id: magicDnsName
            description: "Magic DNS Name"
            text: loginProfile?.networkProfile?.magicDnsName ?? ""
            trailing: ToolButton {
                visible: loginProfile?.networkProfile?.magicDnsName
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.networkProfile.magicDnsName)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: magicDnsName
            above: domainName
        }

        FormCard.FormTextDelegate {
            id: domainName
            text: loginProfile?.networkProfile?.domainName ?? ""
            description: "Domain Name"
            trailing: ToolButton {
                visible: loginProfile?.networkProfile?.domainName
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.networkProfile.domainName)
                }
            }
        }

        FormCard.FormDelegateSeparator {
            below: domainName
            above: displayName
        }

        FormCard.FormTextDelegate {
            id: displayName
            text: loginProfile?.networkProfile?.displayName ?? ""
            description: "Display Name"
            trailing: ToolButton {
                visible: loginProfile?.networkProfile?.displayName
                icon.name: "edit-copy"
                onClicked: {
                    onClicked: KTailctl.Util.setClipboardText(loginProfile.networkProfile.displayName)
                }
            }
        }
    }
}