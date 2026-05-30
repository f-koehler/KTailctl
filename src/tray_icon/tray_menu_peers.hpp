#ifndef KTAILCTL_TRAY_MENU_PEERS_HPP
#define KTAILCTL_TRAY_MENU_PEERS_HPP

#include <QMenu>
#include <QWidget>
#include <qtmetamacros.h>

#include "tailscale/tailscale.hpp"

class TrayMenuPeers : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexMullvadNode = -1;
    const int mRoleIndexDnsName = -1;
    const int mRoleIndexTailscaleIps = -1;
    const int mRoleIndexOs = -1;

public:
    explicit TrayMenuPeers(Tailscale *tailscale, QWidget *parent = nullptr);

    Q_INVOKABLE void rebuildMenu();
};

#endif // KTAILCTL_TRAY_MENU_PEERS_HPP
