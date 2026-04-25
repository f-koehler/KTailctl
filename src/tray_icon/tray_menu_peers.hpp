#ifndef KTAILCTL_TRAY_MENU_PEERS_HPP
#define KTAILCTL_TRAY_MENU_PEERS_HPP

#include "tailscale/tailscale.hpp"
#include "util.hpp"

#include <QListView>
#include <QMenu>

class TrayMenuPeers : public QMenu
{
    Q_OBJECT

private:
    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexMullvadNode = -1;
    const int mRoleIndexDnsName = -1;
    const int mRoleIndexTailscaleIps = -1;

public Q_SLOTS:
    Q_INVOKABLE void rebuildMenu();

public:
    explicit TrayMenuPeers(Tailscale *tailscale, QWidget *parent = nullptr);
};

#endif // KTAILCTL_TRAY_MENU_PEERS_HPP
