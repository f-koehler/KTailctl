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
            label: "Refresh interval (ms)"
            value: KTailctl.Config.refreshInterval
            from: 100
            to: 30000
            onValueChanged: KTailctl.Config.refreshInterval = spinRefreshInterval.value
        }
        FormCard.FormSwitchDelegate {
            id: switchShowTagsInPeerList
            text: "Show tags in peer list"
            checked: KTailctl.Config.showTagsInPeerList
            onCheckedChanged: KTailctl.Config.showTagsInPeerList = switchShowTagsInPeerList.checked
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
            id: switchRouteAll
            enabled: page.isOperator
            text: "Accept routes"
            checked: KTailctl.Tailscale.preferences.routeAll
            onCheckedChanged: KTailctl.Tailscale.preferences.routeAll = switchRouteAll.checked
        }
        FormCard.FormSwitchDelegate {
            id: switchAllowLanAccess
            enabled: page.isOperator
            text: "Allow LAN access"
            checked: KTailctl.Tailscale.preferences.exitNodeAllowLanAccess
            onCheckedChanged: KTailctl.Tailscale.preferences.exitNodeAllowLanAccess = switchAllowLanAccess.checked
        }
        FormCard.FormSwitchDelegate {
            id: switchStatefulFiltering
            enabled: page.isOperator
            text: "Stateful filtering"
            checked: !KTailctl.Tailscale.preferences.noStatefulFiltering
            onCheckedChanged: KTailctl.Tailscale.preferences.noStatefulFiltering = !switchStatefulFiltering.checked
        }
        FormCard.FormComboBoxDelegate {
            id: comboNetfilterMode
            enabled: page.isOperator
            text: "Netfilter mode"
            model: ["Off", "No Divert", "On"]
            currentIndex: KTailctl.Tailscale.preferences.netfilterMode
            onActivated: KTailctl.Tailscale.preferences.setNetfilterMode(currentIndex)
        }
        FormCard.FormSwitchDelegate {
            id: switchAdvertiseAppConnector
            text: "Advertise app connector"
            enabled: false
            checked: KTailctl.Tailscale.preferences.appConnectorPreferences.advertise
        }
    }

    FormCard.FormHeader {
        title: "Management"
    }
    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: switchPostureChecking
            enabled: page.isOperator
            text: "Report device posture"
            checked: KTailctl.Tailscale.preferences.postureChecking
            onCheckedChanged: KTailctl.Tailscale.preferences.postureChecking = switchPostureChecking.checked
        }
        FormCard.FormSwitchDelegate {
            id: switchRunWebClient
            enabled: page.isOperator
            text: "Management web interface"
            checked: KTailctl.Tailscale.preferences.runWebClient
            onCheckedChanged: KTailctl.Tailscale.preferences.runWebClient = switchRunWebClient.checked
        }
        FormCard.FormLinkDelegate {
            url: "http://localhost:5252"
            text: "Open management web interface"
            visible: KTailctl.Tailscale.preferences.runWebClient
        }
        FormCard.FormSwitchDelegate {
            id: switchAutoUpdateCheck
            text: "Automatic update check"
            enabled: false
            checked: KTailctl.Tailscale.preferences.autoUpdate.check
        }
        FormCard.FormSwitchDelegate {
            id: switchAutoUpdate
            text: "Automatic updates"
            enabled: false
            checked: KTailctl.Tailscale.preferences.autoUpdate.apply
        }
    }

    FormCard.FormHeader {
        title: "Relay Server"
        visible: KTailctl.Tailscale.preferences.relayServerPort > 0 || KTailctl.Tailscale.preferences.relayServerStaticEndpoints.length > 0
    }
    FormCard.FormCard {
        visible: KTailctl.Tailscale.preferences.relayServerPort > 0 || KTailctl.Tailscale.preferences.relayServerStaticEndpoints.length > 0

        FormCard.FormSpinBoxDelegate {
            label: "Relay server port"
            enabled: false
            value: KTailctl.Tailscale.preferences.relayServerPort
            from: 0
            to: 65535
        }
        Repeater {
            model: KTailctl.Tailscale.preferences.relayServerStaticEndpoints
            FormCard.FormTextDelegate {
                text: modelData
            }
        }
    }
}
