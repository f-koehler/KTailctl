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
#include "tray_icon/tray_menu_util.hpp"
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
    // Populate eagerly and rebuild only when the data changes (see TrayMenuUtil::makeRebuildTimer)
    QTimer *timer = TrayMenuUtil::makeRebuildTimer(this, [this] -> void {
        rebuildMenu();
    });
    TrayMenuUtil::rebuildOnModelChange(mTailscale->status()->peerModel(), timer);
    rebuildMenu();
}

void TrayMenuPeers::rebuildMenu()
{
    if (!mRoleIndicesFound) {
        return;
    }

    struct Entry {
        QString title;
        QString os;
        QString dnsName;
        QStringList addresses;
    };
    QList<Entry> entries;
    QString signature;

    const int rows = mTailscale->status()->peerModel()->rowCount(QModelIndex{});
    for (int row = 0; row < rows; ++row) {
        const QModelIndex index = mTailscale->status()->peerModel()->index(row, 0);
        if (!index.isValid()) {
            continue;
        }
        if (index.data(mRoleIndexMullvadNode).toBool()) {
            continue;
        }
        Entry entry{.title = index.data(mRoleIndexHostName).toString(),
                    .os = index.data(mRoleIndexOs).toString().toLower(),
                    .dnsName = index.data(mRoleIndexDnsName).toString(),
                    .addresses = index.data(mRoleIndexTailscaleIps).toStringList()};
        signature += entry.title + QLatin1Char('\x1f') + entry.os + QLatin1Char('\x1f') + entry.dnsName + QLatin1Char('\x1f')
            + entry.addresses.join(QLatin1Char(',')) + QLatin1Char('\x1e');
        entries.append(std::move(entry));
    }

    if (mBuilt && signature == mLastSignature) {
        return;
    }
    mBuilt = true;
    mLastSignature = signature;

    clear();
    if (entries.isEmpty()) {
        addAction(tr("No peers"))->setEnabled(false);
        return;
    }
    for (const Entry &entry : std::as_const(entries)) {
        QMenu *subMenu = addMenu(entry.title);

        if ((entry.os == QStringLiteral("android")) || (entry.os == QStringLiteral("ios"))) {
            subMenu->setIcon(QIcon::fromTheme(QStringLiteral("smartphone")));
        } else {
            subMenu->setIcon(QIcon::fromTheme(QStringLiteral("computer")));
        }

        // Populate the submenu completely *before* it is exported, so hosts see children and a stable structure
        const QString dnsName = entry.dnsName;
        subMenu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), dnsName, this, [dnsName] -> void {
            Util::setClipboardText(dnsName);
        });
        for (const auto &address : std::as_const(entry.addresses)) {
            subMenu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), address, this, [address] -> void {
                Util::setClipboardText(address);
            });
        }
    }
}
