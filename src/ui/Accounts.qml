import QtQuick
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kirigamiaddons.labs.components as Components

Kirigami.ScrollablePage {
    id: accounts

    ColumnLayout {
        FormCard.FormCard {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 0

                Repeater {
                    model: Tailscale.accountModel

                    FormCard.FormButtonDelegate {
                        required property QtObject modelData

                        enabled: !modelData.isCurrent
                        icon.name: "user"
                        text: modelData.loginName

                        onClicked: {
                            Tailscale.switchAccount(modelData.loginName);
                        }
                    }
                }
            }
        }
    }
}
