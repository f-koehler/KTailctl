#include "tray_menu_self.hpp"

#include "util.hpp"
#include <QIcon>
#include <QString>

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
