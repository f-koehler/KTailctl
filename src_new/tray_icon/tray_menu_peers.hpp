#ifndef KTAILCTL_TRAY_MENU_PEERS_HPP
#define KTAILCTL_TRAY_MENU_PEERS_HPP
#include "logging_tray_icon.hpp"
#include "util.hpp"

#include <QListView>
#include <QMenu>
#include <QWidgetAction>

class TrayMenuPeers : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexMullvadNode = -1;
    const int mRoleIndexDnsName = -1;
    const int mRoleIndexTailscaleIps = -1;

public slots:
    Q_INVOKABLE void rebuildMenu()
    {
        clear();
        if (!mRoleIndicesFound) {
            return;
        }

        const int rows = mTailscale->status()->peerModel()->rowCount(QModelIndex{});
        for (int row = 0; row < rows; ++row) {
            QModelIndex index = mTailscale->status()->peerModel()->index(row, 0);
            if (!index.isValid()) {
                continue;
            }
            if (index.data(mRoleIndexMullvadNode).toBool()) {
                continue;
            }
            QPersistentModelIndex persistent_index(index);
            const QString title = index.data(mRoleIndexHostName).toString();
            QMenu *subMenu = addMenu(title);

            const QString dnsName = index.data(mRoleIndexDnsName).toString();
            subMenu->addAction(QIcon::fromTheme("edit-copy"), dnsName, [dnsName]() {
                Util::setClipboardText(dnsName);
            });

            for (const auto &address : index.data(mRoleIndexTailscaleIps).toStringList()) {
                subMenu->addAction(QIcon::fromTheme("edit-copy"), address, [address]() {
                    Util::setClipboardText(address);
                });
            }
        }
    }

public:
    explicit TrayMenuPeers(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Peers"), parent)
        , mTailscale(tailscale)
        , mRoleIndexHostName(tailscale->status()->peerModel()->roleIndexForProperty("hostName"))
        , mRoleIndexMullvadNode(tailscale->status()->peerModel()->roleIndexForProperty("mullvadNode"))
        , mRoleIndexDnsName(tailscale->status()->peerModel()->roleIndexForProperty("dnsName"))
        , mRoleIndexTailscaleIps(tailscale->status()->peerModel()->roleIndexForProperty("tailscaleIps"))
    {
        mRoleIndicesFound = true;
        if (mRoleIndexHostName == -1) {
            qCCritical(Logging::TrayIcon) << "Failed to find role index for hostName";
            mRoleIndicesFound = false;
        }
        if (mRoleIndexHostName == -1) {
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
};

#endif // KTAILCTL_TRAY_MENU_PEERS_HPP
