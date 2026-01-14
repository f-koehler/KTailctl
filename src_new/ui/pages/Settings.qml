import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import QtQml.Models
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    id: page

    Layout.fillWidth: true

    FormCard.FormHeader {
        title: "Interface"
    }
    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            text: "Start minimized"
        }
        FormCard.FormDelegateSeparator {}
        FormCard.FormSwitchDelegate {
            text: "Tray icon"
        }
        FormCard.FormDelegateSeparator {}
        FormCard.FormSpinBoxDelegate {
            label: "Refresh interval"
        }
    }

    FormCard.FormHeader {
        title: "Tailscale"
    }
    FormCard.FormCard {
        FormCard.FormTextFieldDelegate {
            label: Tailscale.preferences.hostname
        }
        FormCard.FormSwitchDelegate {
            text: "SSH"
        }
        FormCard.FormSwitchDelegate {
            text: "Shields up"
        }
        FormCard.FormSwitchDelegate {
            text: "Accept DNS"
            value: true
        }
    }

    FormCard.FormHeader {
        title: "Exit Node"
    }
    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            text: "Advertise exit node"
        }
        FormCard.FormSwitchDelegate {
            text: "LAN access while using exit node"
        }
    }

    FormCard.FormHeader {
        title: "Routing & Firewall"
    }
    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            text: "Accept routes"
        }
        FormCard.FormSwitchDelegate {
            text: "Advertise app connector"
        }
        FormCard.FormSwitchDelegate {
            text: "Stateful filtering"
        }
        FormCard.FormComboBoxDelegate {
            text: "Netfilter mode"
            model: ["On", "Off", "Divert"]
        }
    }

    FormCard.FormHeader {
        title: "Management"
    }
    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            text: "Report device posture"
        }
        FormCard.FormSwitchDelegate {
            text: "Enable management web interface"
        }
        FormCard.FormLinkDelegate {
            url: "localhost:5252"
            text: "Go to management web interface (localhost:5252)"
        }
        FormCard.FormSwitchDelegate {
            text: "Automatic update check"
        }
        FormCard.FormSwitchDelegate {
            text: "Automatic updates"
        }
    }

    FormCard.FormHeader {
        title: "Relay Server"
    }
    FormCard.FormCard {
        FormCard.FormSpinBoxDelegate {
            label: "Relay server port"
        }
        Repeater {
            model: ["[2001:db8::1]:40000","192.0.2.1:40000"]
            FormCard.FormButtonDelegate {
                text: modelData
                trailingLogo.source: "remove"
            }
        }
        FormCard.FormTextFieldDelegate {
            label: "Add new: "
        }
    }

    // TODO: advertise routes
    // TODO: relay server endpoints and port
    // snat subnet routes

    // TODO: add to current loginprofile: FormCard.FormTextFieldDelegate {
    //     label: "Account Nickname"
    // }
}