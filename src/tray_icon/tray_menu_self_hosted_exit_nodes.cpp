#include "tray_menu_self_hosted_exit_nodes.hpp"

#include <QByteArray>
#include <QList>
#include <QLoggingCategory>
#include <QMessageLogger>
#include <QModelIndex>
#include <QPair>
#include <QString>
#include <QStringLiteral>
#include <QVariant>

#include "logging_tray_icon.hpp"
#include "preferences.hpp"
#include "self_hosted_exit_node_model.hpp"
#include "status.hpp"
#include "tailscale.hpp"
#include "tray_icon/tray_menu_util.hpp"

TrayMenuExitNodesSelfHosted::TrayMenuExitNodesSelfHosted(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Self-Hosted"), parent)
    , mTailscale(tailscale)
    , mRoleIndicesFound(true)
    , mRoleIndexHostName((tailscale->status()->peerModel()->roleIndexForProperty("hostName")))
    , mRoleIndexId((tailscale->status()->peerModel()->roleIndexForProperty("id")))
{
    if (mRoleIndexHostName == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for hostName";
        mRoleIndicesFound = false;
    }
    if (mRoleIndexId == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for id";
        mRoleIndicesFound = false;
    }
    QTimer *timer = TrayMenuUtil::makeRebuildTimer(this, [this] -> void {
        rebuildMenu();
    });
    TrayMenuUtil::rebuildOnModelChange(mTailscale->status()->selfHostedExitNodeModel(), timer);
    rebuildMenu();
}

void TrayMenuExitNodesSelfHosted::rebuildMenu()
{
    if (!mRoleIndicesFound) {
        return;
    }
    QList<QPair<QString, QString>> nodes; // (id, host name)
    QString signature;
    const int rows = mTailscale->status()->selfHostedExitNodeModel()->rowCount(QModelIndex{});
    for (int row = 0; row < rows; ++row) {
        const QModelIndex index = mTailscale->status()->selfHostedExitNodeModel()->index(row, 0);
        if (!index.isValid()) {
            continue;
        }
        nodes.append({index.data(mRoleIndexId).toString(), index.data(mRoleIndexHostName).toString()});
        signature += nodes.last().first + QLatin1Char('\x1f') + nodes.last().second + QLatin1Char('\x1e');
    }
    if (mBuilt && signature == mLastSignature) {
        return;
    }
    mBuilt = true;
    mLastSignature = signature;

    clear();
    setEnabled(!nodes.isEmpty());
    if (nodes.isEmpty()) {
        addAction(tr("No self-hosted exit nodes"))->setEnabled(false);
        return;
    }
    for (const auto &[exitNodeId, hostName] : std::as_const(nodes)) {
        addAction(hostName, this, [this, exitNodeId] -> void {
            mTailscale->preferences()->setExitNodeID(exitNodeId);
        });
    }
}
