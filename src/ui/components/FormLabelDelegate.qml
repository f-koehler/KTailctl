import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.fkoehler.KTailctl as KTailctl
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.AbstractFormDelegate {
    id: root

    property string label

    focusPolicy: Qt.StrongFocus

    contentItem: RowLayout {
        spacing: Kirigami.Units.largeSpacing

        Controls.Label {
            color: root.enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            elide: Text.ElideRight
            maximumLineCount: 2
            text: root.text
            wrapMode: Text.Wrap
        }

        Item {
            Layout.fillWidth: true
        }

        Controls.Label {
            color: root.enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
            elide: Text.ElideRight
            maximumLineCount: 2
            text: root.label
            wrapMode: Text.Wrap
        }

        Item {
            implicitWidth: Kirigami.Units.iconSizes.small
        }
    }
}
