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
            checked: KTailctl.Config.startMinimized
            onCheckedChanged: {
                KTailctl.Config.startMinimized = !KTailctl.Config.startMinimized
                if(KTailctl.Config.startMinimized) {
                    KTailctl.Config.enableTrayIcon = true;
                }
            }
        }
        FormCard.FormDelegateSeparator {}
        FormCard.FormSwitchDelegate {
            text: "Tray icon"
            enabled: !KTailctl.Config.startMinimized
            checked: KTailctl.Config.enableTrayIcon
            onCheckedChanged: KTailctl.Config.enableTrayIcon = !KTailctl.Config.enableTrayIcon
        }
        FormCard.FormDelegateSeparator {}
        FormCard.FormSpinBoxDelegate {
            id: spinRefreshInterval
            label: "Refresh interval"
            value: KTailctl.Config.refreshInterval
            from: 100
            to: 30000
            onValueChanged: KTailctl.Config.refreshInterval = spinRefreshInterval.value
        }
    }

    FormCard.FormHeader {
        title: "Tailscale"
    }
    FormCard.FormCard {
        FormCard.FormTextFieldDelegate {
            label: "Hostname"
            text: KTailctl.Tailscale.preferences.hostname
        }
        FormCard.FormSwitchDelegate {
            text: "SSH"
            checked: KTailctl.Tailscale.preferences.runSSH
        }
        FormCard.FormSwitchDelegate {
            text: "Shields up"
            checked: KTailctl.Tailscale.preferences.shieldsUp
        }
        FormCard.FormSwitchDelegate {
            text: "Accept DNS"
            checked: KTailctl.Tailscale.preferences.corpDns
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