import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0 as KTailctl
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard

FormCard.AbstractFormDelegate {
    id: root

    property string label
    property string source

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

        Kirigami.Icon {
            implicitHeight: Kirigami.Units.iconSizes.small
            implicitWidth: Kirigami.Units.iconSizes.small
            source: root.source
        }
    }
}
