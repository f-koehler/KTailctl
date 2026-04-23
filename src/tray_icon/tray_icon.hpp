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

private:
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

signals:
    void showWindow();
    void quitRequested();

public slots:
    Q_INVOKABLE void updateIcon();

public:
    explicit TrayIcon(Tailscale *tailscale, QObject *parent = nullptr);
};

#endif // KTAILCTL_TRAY_ICON_HPP
