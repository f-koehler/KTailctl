#ifndef KTAILCTL_TRAY_MENU_PEERS_HPP
#define KTAILCTL_TRAY_MENU_PEERS_HPP

#include <QMenu>
#include <QString>
#include <QWidget>
#include <qtmetamacros.h>

#include "tailscale/tailscale.hpp"

class TrayMenuPeers : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    QString mLastSignature;
    bool mBuilt = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexMullvadNode = -1;
    const int mRoleIndexDnsName = -1;
    const int mRoleIndexTailscaleIps = -1;
    const int mRoleIndexOs = -1;
    const int mRoleIndexId = -1;
    const int mRoleIndexTaildropTargetStatus = -1;

public:
    explicit TrayMenuPeers(Tailscale *tailscale, QWidget *parent = nullptr);

    Q_INVOKABLE void rebuildMenu();

private:
    void openSendFilesDialog(const QString &peerId, const QString &peerName);
};

#endif // KTAILCTL_TRAY_MENU_PEERS_HPP
