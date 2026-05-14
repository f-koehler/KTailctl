import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl

Kirigami.InlineMessage {
    readonly property string systemUser: KTailctl.Util.systemUser()
    readonly property bool isOperator: {
        const op = KTailctl.Tailscale.preferences.operatorUser;
        return op !== "" && op === systemUser;
    }

    Layout.fillWidth: true
    type: Kirigami.MessageType.Warning
    showCloseButton: true
    visible: !isOperator
    text: i18n("You are not the Tailscale operator, so changes to settings, exit nodes, and the Tailscale daemon are disabled. To grant yourself operator access, run: <tt>sudo tailscale set --operator=%1</tt>", systemUser)
    actions: [
        Kirigami.Action {
            icon.name: "edit-copy"
            text: i18n("Copy command")
            onTriggered: KTailctl.Util.setClipboardText("sudo tailscale set --operator=" + systemUser)
        }
    ]
}
