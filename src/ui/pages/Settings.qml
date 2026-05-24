import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import QtQml.Models
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigamiaddons.formcard as FormCard
import "qrc:/ui/components"

FormCard.FormCardPage {
    id: page

    Layout.fillWidth: true

    readonly property bool isOperator: {
        const op = KTailctl.Tailscale.preferences.operatorUser;
        return op !== "" && op === KTailctl.Util.systemUser();
    }

    DaemonError {}

    OperatorWarning {}

    AuthError {}

    FormCard.FormHeader {
        title: "Interface"
    }
    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: switchStartMinimized
            text: "Start minimized"
            checked: KTailctl.Config.startMinimized
            onCheckedChanged: {
                KTailctl.Config.startMinimized = switchStartMinimized.checked;
                if (KTailctl.Config.startMinimized) {
                    KTailctl.Config.enableTrayIcon = true;
                }
            }
        }
        FormCard.FormSwitchDelegate {
            id: switchTrayIcon
            text: "Tray icon"
            enabled: !KTailctl.Config.startMinimized
            checked: KTailctl.Config.enableTrayIcon
            onCheckedChanged: KTailctl.Config.enableTrayIcon = switchTrayIcon.checked
        }
        FormCard.FormComboBoxDelegate {
            id: comboTrayIconTheme
            text: "Tray icon theme"
            model: KTailctl.TrayIconThemes.themes
            currentIndex: KTailctl.TrayIconThemes.currentIndex
            onActivated: KTailctl.TrayIconThemes.currentIndex = comboTrayIconTheme.currentIndex
        }
        FormCard.FormSpinBoxDelegate {
            id: spinRefreshInterval
            label: "Refresh interval"
            value: KTailctl.Config.refreshInterval
            from: 100
            to: 30000
            onValueChanged: KTailctl.Config.refreshInterval = spinRefreshInterval.value
        }
        FormCard.FormSwitchDelegate {
            id: switchShowTagsInPeerList
            text: "Show tags in peer list"
            checked: KTailctl.Config.showTagsInPeerList
            onCheckedChanged: KTailctl.Config.showTagsInPeerList = !KTailctl.Config.showTagsInPeerList
        }
    }

    FormCard.FormHeader {
        title: "Tailscale"
    }
    FormCard.FormCard {
        FormCard.FormTextFieldDelegate {
            id: textHostname
            enabled: page.isOperator
            label: "Hostname"
            text: KTailctl.Tailscale.preferences.hostname
            onEditingFinished: KTailctl.Tailscale.preferences.hostname = textHostname.text
        }
        FormCard.FormSwitchDelegate {
            id: switchRunSSH
            enabled: page.isOperator
            text: "SSH"
            checked: KTailctl.Tailscale.preferences.runSSH
            onCheckedChanged: KTailctl.Tailscale.preferences.runSSH = switchRunSSH.checked
        }
        FormCard.FormSwitchDelegate {
            id: switchShieldsUp
            enabled: page.isOperator
            text: "Shields up"
            checked: KTailctl.Tailscale.preferences.shieldsUp
            onCheckedChanged: KTailctl.Tailscale.preferences.shieldsUp = switchShieldsUp.checked
        }
        FormCard.FormSwitchDelegate {
            id: switchCorpDns
            enabled: page.isOperator
            text: "Accept DNS"
            checked: KTailctl.Tailscale.preferences.corpDns
            onCheckedChanged: KTailctl.Tailscale.preferences.corpDns = switchCorpDns.checked
        }
    }

    FormCard.FormHeader {
        title: "Routing & Firewall"
    }
    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            enabled: page.isOperator
            text: "Accept routes"
        }
        FormCard.FormSwitchDelegate {
            id: switchAllowLanAccess
            enabled: page.isOperator
            text: "Allow LAN access"
            checked: KTailctl.Tailscale.preferences.exitNodeAllowLanAccess
            onCheckedChanged: KTailctl.Tailscale.preferences.exitNodeAllowLanAccess = switchAllowLanAccess.checked
        }
        FormCard.FormSwitchDelegate {
            enabled: page.isOperator
            text: "Advertise app connector"
        }
        FormCard.FormSwitchDelegate {
            enabled: page.isOperator
            text: "Stateful filtering"
        }
        FormCard.FormComboBoxDelegate {
            enabled: page.isOperator
            text: "Netfilter mode"
            model: ["On", "Off", "Divert"]
        }
    }

    FormCard.FormHeader {
        title: "Management"
    }
    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            enabled: page.isOperator
            text: "Report device posture"
        }
        FormCard.FormSwitchDelegate {
            enabled: page.isOperator
            text: "Enable management web interface"
        }
        FormCard.FormLinkDelegate {
            url: "localhost:5252"
            text: "Go to management web interface (localhost:5252)"
        }
        FormCard.FormSwitchDelegate {
            enabled: page.isOperator
            text: "Automatic update check"
        }
        FormCard.FormSwitchDelegate {
            enabled: page.isOperator
            text: "Automatic updates"
        }
    }

    FormCard.FormHeader {
        title: "Relay Server"
    }
    FormCard.FormCard {
        FormCard.FormSpinBoxDelegate {
            enabled: page.isOperator
            label: "Relay server port"
        }
        Repeater {
            model: ["[2001:db8::1]:40000", "192.0.2.1:40000"]
            FormCard.FormButtonDelegate {
                enabled: page.isOperator
                text: modelData
                trailingLogo.source: "remove"
            }
        }
        FormCard.FormTextFieldDelegate {
            enabled: page.isOperator
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
