import org.kde.kirigamiaddons.formcard as FormCard
import org.fkoehler.KTailctl as KTailctl
import QtQuick

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
        }

        FormCard.FormDelegateSeparator {
            below: loginProfileId
            above: name
        }

        FormCard.FormTextDelegate {
            id: name
            text: loginProfile?.name ?? ""
        }

        FormCard.FormDelegateSeparator {
            below: name
            above: key
        }

        FormCard.FormTextDelegate {
            id: key
            text: loginProfile?.key ?? ""
        }

        FormCard.FormDelegateSeparator {
            below: key
            above: nodeId
        }

        FormCard.FormTextDelegate {
            id: nodeId
            text: loginProfile?.nodeId ?? ""
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
        }
    }

    FormCard.FormHeader {
        title: "User Profile"
    }

    FormCard.FormCard {
        FormCard.FormTextDelegate {
            id: userId
            text: loginProfile?.userProfile?.userId ?? ""
        }

        FormCard.FormDelegateSeparator {
            below: userId
            above: loginName
        }

        FormCard.FormTextDelegate {
            id: loginName
            text: loginProfile?.userProfile?.loginName ?? ""
        }
    }

    FormCard.FormHeader {
        title: "Network Profile Profile"
    }

    FormCard.FormCard {
        FormCard.FormTextDelegate {
            id: magicDnsName
            text: loginProfile?.networkProfile?.magicDnsName ?? ""
        }

        FormCard.FormDelegateSeparator {
            below: magicDnsName
            above: domainName
        }

        FormCard.FormTextDelegate {
            id: domainName
            text: loginProfile?.networkProfile?.domainName ?? ""
        }

        FormCard.FormDelegateSeparator {
            below: domainName
            above: displayName
        }

        FormCard.FormTextDelegate {
            id: displayName
            text: loginProfile?.networkProfile?.displayName ?? ""
        }
    }
}