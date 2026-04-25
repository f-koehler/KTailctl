#include "tray_menu_peers.hpp"
#include "logging_tray_icon.hpp"
#include <qhashfunctions.h>

TrayMenuPeers::TrayMenuPeers(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Peers"), parent)
    , mTailscale(tailscale)
    , mRoleIndicesFound(true)
    , mRoleIndexHostName(tailscale->status()->peerModel()->roleIndexForProperty("hostName"))
    , mRoleIndexMullvadNode(tailscale->status()->peerModel()->roleIndexForProperty("mullvadNode"))
    , mRoleIndexDnsName(tailscale->status()->peerModel()->roleIndexForProperty("dnsName"))
    , mRoleIndexTailscaleIps(tailscale->status()->peerModel()->roleIndexForProperty("tailscaleIps"))
{
    if (mRoleIndexHostName == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for hostName";
        mRoleIndicesFound = false;
    }
    if (mRoleIndexMullvadNode == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for mullvadNode";
        mRoleIndicesFound = false;
    }
    if (mRoleIndexDnsName == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for dnsName";
        mRoleIndicesFound = false;
    }
    if (mRoleIndexTailscaleIps == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for tailscaleIps";
        mRoleIndicesFound = false;
    }
    connect(this, &QMenu::aboutToShow, this, &TrayMenuPeers::rebuildMenu);
}

void TrayMenuPeers::rebuildMenu()
{
    clear();
    if (!mRoleIndicesFound) {
        return;
    }

    const int rows = mTailscale->status()->peerModel()->rowCount(QModelIndex{});
    for (int row = 0; row < rows; ++row) {
        const QModelIndex index = mTailscale->status()->peerModel()->index(row, 0);
        if (!index.isValid()) {
            continue;
        }
        if (index.data(mRoleIndexMullvadNode).toBool()) {
            continue;
        }
        const QString title = index.data(mRoleIndexHostName).toString();
        QMenu *subMenu = addMenu(title);

        const QString dnsName = index.data(mRoleIndexDnsName).toString();
        subMenu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), dnsName, [dnsName] {
            Util::setClipboardText(dnsName);
        });

        for (const auto &address : index.data(mRoleIndexTailscaleIps).toStringList()) {
            subMenu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), address, [address] {
                Util::setClipboardText(address);
            });
        }
    }
}
