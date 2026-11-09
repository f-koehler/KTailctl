#ifndef KTAILCTL_TRAY_MENU_SELF_HPP
#define KTAILCTL_TRAY_MENU_SELF_HPP

#include <QIcon>
#include <QMenu>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QStringLiteral>
#include <QTimer>
#include <QWidget>
#include <qtmetamacros.h>

#include "tailscale/status/peer_status.hpp"
#include "tailscale/status/status.hpp"
#include "tailscale/tailscale.hpp"

class TrayMenuSelf : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    QTimer *mRebuildTimer;
    QPointer<PeerStatus> mWatchedSelf;
    QStringList mLastEntries;
    bool mBuilt = false;

public:
    explicit TrayMenuSelf(Tailscale *tailscale, QWidget *parent = nullptr);

    Q_SLOT Q_INVOKABLE void rebuildMenu();

private:
    void watchSelf();
};

#endif // KTAILCTL_TRAY_MENU_SELF_HPP
