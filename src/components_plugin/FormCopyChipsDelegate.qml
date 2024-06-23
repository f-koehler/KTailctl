import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0 as KTailctl
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard

FormCard.AbstractFormDelegate {
    id: root

    property string copyData
    property var model

    focusPolicy: Qt.StrongFocus

    contentItem: RowLayout {
        spacing: Kirigami.Units.largeSpacing

        Controls.Label {
            Layout.fillWidth: true
            text: root.text
            elide: Text.ElideRight
            wrapMode: Text.Wrap
            maximumLineCount: 2
            color: root.enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
        }

        Flow {
            spacing: Kirigami.Units.smallSpacing

            Repeater {
                model: root.model

                delegate: Kirigami.Chip {
                    text: modelData
                    icon.name: "edit-copy"
                    closable: false
                    checkable: false
                    checked: false
                    onClicked: {
                        KTailctl.Util.setClipboardText(modelData);
                    }
                }

            }

        }

    }

}
