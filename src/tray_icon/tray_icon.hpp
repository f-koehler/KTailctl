#ifndef KTAILCTL_TRAY_ICON_HPP
#define KTAILCTL_TRAY_ICON_HPP

#include "./tray_menu_accounts.hpp"
#include "./tray_menu_exit_nodes.hpp"
#include "./tray_menu_peers.hpp"
#include "./tray_menu_self.hpp"
#include "ktailctl_config.h"
#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

    Tailscale *mTailscale;
    QMenu *mContextMenu;
    TrayMenuAccounts *mMenuAccounts;
    TrayMenuSelf *mMenuSelf;
    TrayMenuPeers *mMenuPeers;
    TrayMenuExitNodes *mMenuExitNodes;
    QAction *mActionShow;
    QAction *mActionStart;
    QAction *mActionStop;
    QAction *mActionQuit;

Q_SIGNALS:
    void showWindow();
    void quitRequested();

public:
    explicit TrayIcon(Tailscale *tailscale, QObject *parent = nullptr);

public Q_SLOTS:
    Q_INVOKABLE void updateIcon();
};

#endif // KTAILCTL_TRAY_ICON_HPP
