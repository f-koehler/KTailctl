import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigami as Kirigami
import QtQuick
import QtQuick.Controls

FormCard.FormCardPage {
    property KTailctl.LoginProfile loginProfile: null

    Component.onCompleted: {
        console.log("Login profile:", loginProfile);
    }

    FormCard.FormHeader {
        title: "Login Profile"
    }

    FormCard.FormCard {
        FormCard.FormButtonDelegate {
            id: loginProfileId
            text: loginProfile?.id ?? ""
            description: "ID"
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(loginProfile.id)
        }

        FormCard.FormDelegateSeparator {
            below: loginProfileId
            above: name
        }

        FormCard.FormButtonDelegate {
            id: name
            text: loginProfile?.name ?? ""
            description: "Name"
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(loginProfile.name)
        }

        FormCard.FormDelegateSeparator {
            below: name
            above: key
        }

        FormCard.FormButtonDelegate {
            id: key
            text: loginProfile?.key ?? ""
            description: "Key"
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(loginProfile.key)
        }

        FormCard.FormDelegateSeparator {
            below: key
            above: nodeId
        }

        FormCard.FormButtonDelegate {
            id: nodeId
            text: loginProfile?.nodeId ?? ""
            description: "Node ID"
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(loginProfile.nodeId)
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

        FormCard.FormLinkDelegate {
            id: controlUrl
            url: loginProfile?.controlUrl ?? ""
            text: loginProfile?.controlUrl ?? ""
            description: "Control Pane URL"
        }
    }

    FormCard.FormHeader {
        title: "User Profile"
    }

    FormCard.FormCard {
        FormCard.FormButtonDelegate {
            id: userId
            text: toString(loginProfile?.userProfile?.userId) ?? ""
            description: "User ID"
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(toString(loginProfile.userProfile.userId))
        }

        FormCard.FormDelegateSeparator {
            below: userId
            above: loginName
        }

        FormCard.FormButtonDelegate {
            id: loginName
            description: "Login Name"
            text: loginProfile?.userProfile?.loginName ?? ""
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(loginProfile.userProfile.loginName)
        }

        FormCard.FormDelegateSeparator {
            below: loginName
            above: profilePic
        }

        FormCard.FormLinkDelegate {
            id: profilePic
            text: loginProfile?.userProfile?.profilePicUrl ?? ""
            url: loginProfile?.userProfile?.profilePicUrl ?? ""
            description: "Profile Picture"
            leading: Kirigami.Icon {
                ToolTip.delay: Kirigami.Units.toolTipDelay
                ToolTip.text: "Profile Picture"
                ToolTip.visible: hovered
                source: loginProfile?.userProfile?.profilePicUrl
            }
        }
    }

    FormCard.FormHeader {
        title: "Network Profile Profile"
    }

    FormCard.FormCard {
        FormCard.FormButtonDelegate {
            id: magicDnsName
            description: "Magic DNS Name"
            text: loginProfile?.networkProfile?.magicDnsName ?? ""
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(loginProfile.networkProfile.magicDnsName)
        }

        FormCard.FormDelegateSeparator {
            below: magicDnsName
            above: domainName
        }

        FormCard.FormButtonDelegate {
            id: domainName
            text: loginProfile?.networkProfile?.domainName ?? ""
            description: "Domain Name"
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(loginProfile.networkProfile.domainName)
        }

        FormCard.FormDelegateSeparator {
            below: domainName
            above: displayName
        }

        FormCard.FormButtonDelegate {
            id: displayName
            text: loginProfile?.networkProfile?.displayName ?? ""
            description: "Display Name"
            trailingLogo.source: "edit-copy"
            onClicked: KTailctl.Util.setClipboardText(loginProfile.networkProfile.displayName)
        }
    }
}
