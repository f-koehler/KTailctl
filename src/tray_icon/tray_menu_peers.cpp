#include "tray_menu_peers.hpp"

#include <QByteArray>
#include <QIcon>
#include <QList>
#include <QLoggingCategory>
#include <QMessageLogger>
#include <QModelIndex>
#include <QString>
#include <QStringList>
#include <QStringLiteral>
#include <QVariant>
#include <qobject.h>
#include <utility>

#include "logging_tray_icon.hpp"
#include "status.hpp"
#include "tailscale.hpp"
#include "util.hpp"

TrayMenuPeers::TrayMenuPeers(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Peers"), parent)
    , mTailscale(tailscale)
    , mRoleIndicesFound(true)
    , mRoleIndexHostName(tailscale->status()->peerModel()->roleIndexForProperty("hostName"))
    , mRoleIndexMullvadNode(tailscale->status()->peerModel()->roleIndexForProperty("mullvadNode"))
    , mRoleIndexDnsName(tailscale->status()->peerModel()->roleIndexForProperty("dnsName"))
    , mRoleIndexTailscaleIps(tailscale->status()->peerModel()->roleIndexForProperty("tailscaleIps"))
    , mRoleIndexOs(tailscale->status()->peerModel()->roleIndexForProperty("os"))
{
    setIcon(QIcon::fromTheme(QStringLiteral("distribute-graph-directed")));
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
    if (mRoleIndexOs == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for os";
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

        const auto os = index.data(mRoleIndexOs).toString().toLower();
        if ((os == QStringLiteral("android")) || (os == QStringLiteral("ios"))) {
            subMenu->setIcon(QIcon::fromTheme(QStringLiteral("smartphone")));
        } else {
            subMenu->setIcon(QIcon::fromTheme(QStringLiteral("computer")));
        }

        const QString dnsName = index.data(mRoleIndexDnsName).toString();
        subMenu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), dnsName, this, [dnsName] -> void {
            Util::setClipboardText(dnsName);
        });

        const QStringList addresses = index.data(mRoleIndexTailscaleIps).toStringList();
        for (const auto &address : std::as_const(addresses)) {
            subMenu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), address, this, [address] -> void {
                Util::setClipboardText(address);
            });
        }
    }
}
