import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.fkoehler.KTailctl as KTailctl

Kirigami.InlineMessage {
    Layout.fillWidth: true
    type: Kirigami.MessageType.Error
    visible: !KTailctl.Tailscale.status.daemonRunning
    text: i18n("The Tailscale daemon is not running. Follow instructions for your distribution to make sure it is installed and running.")
}
