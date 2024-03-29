import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.fkoehler.KTailctl 1.0 as KTailctl
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard

FormCard.AbstractFormDelegate {
    id: root

    property string copyData

    focusPolicy: Qt.StrongFocus
    onClicked: {
        KTailctl.Util.setClipboardText(root.copyData);
    }

    contentItem: RowLayout {
        spacing: Kirigami.Units.largeSpacing

        Controls.Label {
            text: root.text
            elide: Text.ElideRight
            wrapMode: Text.Wrap
            maximumLineCount: 2
            color: root.enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
        }

        Item {
            Layout.fillWidth: true
        }

        Controls.Label {
            text: root.copyData
            elide: Text.ElideRight
            wrapMode: Text.Wrap
            maximumLineCount: 2
            color: root.enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
        }

        Kirigami.Icon {
            implicitWidth: Kirigami.Units.iconSizes.small
            implicitHeight: Kirigami.Units.iconSizes.small
            source: "edit-copy"
        }

    }

}
