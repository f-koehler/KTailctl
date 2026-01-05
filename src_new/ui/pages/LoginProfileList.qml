import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl

Kirigami.ScrollablePage {
    Layout.fillWidth: true

    Component {
        id: pageLoginProfileInfo

        LoginProfileInfo {
        }
    }

    ListView {
        anchors.fill: parent
        model: KTailctl.Tailscale.loginProfiles

        delegate: ItemDelegate {
            width: ListView.view.width
            id: delegate

            contentItem: RowLayout {
                Kirigami.Icon {
                    ToolTip.delay: Kirigami.Units.toolTipDelay
                    ToolTip.text: "User avatar"
                    ToolTip.visible: hovered
                    source: userProfile.profilePicUrl
                }
                Label {
                    text: name
                }
                Item {
                    Layout.fillWidth: true
                }
                RowLayout {
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
        }
    }
}