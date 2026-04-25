#include "tray_menu_self.hpp"

#include "util.hpp"

void TrayMenuSelf::rebuildMenu()
{
    clear();
    const QString &dnsName = mTailscale->status()->self()->dnsName();
    addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), dnsName, [&dnsName] {
        Util::setClipboardText(dnsName);
    });
    for (const QString &tailscaleIp : mTailscale->status()->self()->tailscaleIps()) {
        addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), tailscaleIp, [&tailscaleIp] {
            Util::setClipboardText(tailscaleIp);
        });
    }
}
