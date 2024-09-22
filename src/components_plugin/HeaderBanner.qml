// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Fabian Köhler <me@fkoehler.org>
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kirigamiaddons.labs.components 1.0 as Components
import org.fkoehler.KTailctl 1.0

Components.Banner {
    id: root

    text: {
        if (!Tailscale.success) {
            return i18n("Tailscaled is not running!");
        }
        if (Tailscale.backendState == "NeedsLogin") {
            return i18n("Please login to Tailscale on the command line!");
        }
        return i18n("KTailctl functionality limited, current user is not the Tailscale operator.");
    }
    type: {
        if (!Tailscale.success) {
            return Kirigami.MessageType.Error;
        }
        if (Tailscale.backendState == "NeedsLogin") {
            return Kirigami.MessageType.Error;
        }
        return Kirigami.MessageType.Warning;
    }
    visible: !Tailscale.success || (Tailscale.backendState == "NeedsLogin") || !Tailscale.isOperator
    width: parent.width

    actions: [
        Kirigami.Action {
            icon.name: "edit-copy"
            text: i18n("Copy command to fix")
            visible: (Tailscale.backendState == "NeedsLogin") || !Tailscale.isOperator

            onTriggered: {
                if (Tailscale.backendState == "NeedsLogin") {
                    Util.setClipboardText("sudo tailscale login --operator=" + Util.getUsername());
                } else if (!Tailscale.isOperator) {
                    Util.setClipboardText("sudo tailscale set --operator=" + Util.getUsername());
                }
            }
        }
    ]
}
