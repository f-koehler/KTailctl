import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    Layout.fillWidth: true

    Component {
        id: pageLoginProfileInfo

        LoginProfileInfo {
        }
    }

    FormCard.FormCard {
        Repeater {
            model: KTailctl.Tailscale.loginProfiles

            delegate: ColumnLayout {
                FormCard.FormTextDelegate {
                    text: name
                    leading: Kirigami.Icon {
                        ToolTip.delay: Kirigami.Units.toolTipDelay
                        ToolTip.text: "User avatar"
                        ToolTip.visible: hovered
                        source: userProfile.profilePicUrl
                    }
                    trailing: RowLayout {
                        ToolButton {
                            ToolTip.delay: Kirigami.Units.toolTipDelay
                            ToolTip.text: "Switch to this account"
                            ToolTip.visible: hovered
                            icon.name: "system-switch-user"
                        }
                        ToolButton {
                            ToolTip.delay: Kirigami.Units.toolTipDelay
                            ToolTip.text: "View account information"
                            ToolTip.visible: hovered
                            icon.name: "help-info"
                            onClicked: {
                                applicationWindow().pageStack.layers.push(pageLoginProfileInfo,
                                    {
                                        loginProfile: KTailctl.Tailscale.loginProfileWithId(id)
                                    }
                                )
                            }
                        }
                    }
                }

                FormCard.FormDelegateSeparator {
                    visible: index < KTailctl.Tailscale.loginProfiles.rowCount() - 1
                }
            }
        }
    }
}