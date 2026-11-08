#include "tray_menu_self.hpp"

#include <QIcon>
#include <QList>
#include <QString>

#include "peer_status.hpp"
#include "status.hpp"
#include "tailscale.hpp"
#include "util.hpp"

void TrayMenuSelf::rebuildMenu()
{
    clear();
    const PeerStatus *self = mTailscale->status()->self();
    if (self == nullptr) {
        // Not logged in / daemon not running: there is no self node
        addAction(tr("Not connected"))->setEnabled(false);
        return;
    }
    const QString dnsName = self->dnsName();
    addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), dnsName, this, [dnsName] -> void {
        Util::setClipboardText(dnsName);
    });
    for (const QString &tailscaleIp : self->tailscaleIps()) {
        addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), tailscaleIp, this, [tailscaleIp] -> void {
            Util::setClipboardText(tailscaleIp);
        });
    }
}
