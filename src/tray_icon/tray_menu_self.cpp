#include "tray_menu_self.hpp"

#include <QIcon>
#include <QList>
#include <QString>

#include "peer_status.hpp"
#include "status.hpp"
#include "tailscale.hpp"
#include "tray_icon/tray_menu_util.hpp"
#include "util.hpp"

TrayMenuSelf::TrayMenuSelf(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Self"), parent)
    , mTailscale(tailscale)
    , mRebuildTimer(TrayMenuUtil::makeRebuildTimer(this, [this] -> void {
        rebuildMenu();
    }))
{
    setIcon(QIcon::fromTheme(QStringLiteral("computer")));

    const auto schedule = [this] -> void {
        mRebuildTimer->start();
    };
    connect(mTailscale->status(), &Status::selfChanged, this, [this, schedule] -> void {
        watchSelf();
        schedule();
    });
    connect(mTailscale->status(), &Status::backendStateChanged, this, schedule);
    watchSelf();
    rebuildMenu();
}

void TrayMenuSelf::watchSelf()
{
    PeerStatus *self = mTailscale->status()->self();
    if (mWatchedSelf == self) {
        return;
    }
    if (mWatchedSelf != nullptr) {
        disconnect(mWatchedSelf, nullptr, this, nullptr);
    }
    mWatchedSelf = self;
    if (self != nullptr) {
        const auto schedule = [this] -> void {
            mRebuildTimer->start();
        };
        connect(self, &PeerStatus::dnsNameChanged, this, schedule);
        connect(self, &PeerStatus::tailscaleIpsChanged, this, schedule);
    }
}

// The menu is populated eagerly and only rebuilt when its content changes. DBusMenu hosts (e.g. the GNOME AppIndicator extension) never open an
// empty submenu and close submenus whose items get recreated after `aboutToShow`.
void TrayMenuSelf::rebuildMenu()
{
    QStringList entries;
    const PeerStatus *self = mTailscale->status()->self();
    if (self != nullptr) {
        entries << self->dnsName();
        entries << self->tailscaleIps();
    }
    if (mBuilt && entries == mLastEntries) {
        return;
    }
    mBuilt = true;
    mLastEntries = entries;

    clear();
    if (self == nullptr) {
        // Not logged in / daemon not running: there is no self node
        addAction(tr("Not connected"))->setEnabled(false);
        return;
    }
    for (const QString &entry : std::as_const(entries)) {
        addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), entry, this, [entry] -> void {
            Util::setClipboardText(entry);
        });
    }
}
