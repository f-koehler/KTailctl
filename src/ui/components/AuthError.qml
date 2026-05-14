import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl

Kirigami.InlineMessage {
    readonly property string systemUser: KTailctl.Util.systemUser()

    Layout.fillWidth: true
    type: Kirigami.MessageType.Error
    visible: (KTailctl.Tailscale.status.backendState == KTailctl.Status.NeedsLogin) && (KTailctl.Tailscale.status.backendState == KTailctl.Status.NeedsMachineAuth)
    text: i18n("You need to login/authenticate with tailscale. Please run: <tt>sudo tailscale login --operator=%1</tt>", systemUser)
    actions: [
        Kirigami.Action {
            icon.name: "edit-copy"
            text: i18n("Copy command")
            onTriggered: KTailctl.Util.setClipboardText("sudo tailscale login --operator=" + systemUser)
        }
    ]
}
