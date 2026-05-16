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
    const QString &dnsName = mTailscale->status()->self()->dnsName();
    addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), dnsName, this, [dnsName] -> void {
        Util::setClipboardText(dnsName);
    });
    for (const QString &tailscaleIp : mTailscale->status()->self()->tailscaleIps()) {
        addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), tailscaleIp, this, [tailscaleIp] -> void {
            Util::setClipboardText(tailscaleIp);
        });
    }
}
