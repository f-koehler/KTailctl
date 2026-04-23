#include "tray_menu_self.hpp"

#include "util.hpp"

void TrayMenuSelf::rebuildMenu()
{
    clear();
    const QString &dnsName = mTailscale.status()->self()->dnsName();
    addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), dnsName, [&dnsName]() {
        Util::setClipboardText(dnsName);
    });
    const QStringList &tailscaleIps = mTailscale.status()->self()->tailscaleIps();
    for (const QString &ip : tailscaleIps) {
        addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), ip, [&ip]() {
            Util::setClipboardText(ip);
        });
    }
}
