#include "tray_menu_self_hosted_exit_nodes.hpp"

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
    connect(this, &QMenu::aboutToShow, this, &TrayMenuExitNodesSelfHosted::rebuildMenu);
}

void TrayMenuExitNodesSelfHosted::rebuildMenu()
{
    clear();
    if (!mRoleIndicesFound) {
        return;
    }
    const int rows = mTailscale->status()->selfHostedExitNodeModel()->rowCount(QModelIndex{});
    if (rows <= 0) {
        setEnabled(false);
        return;
    }
    for (int row = 0; row < rows; ++row) {
        const QModelIndex index = mTailscale->status()->selfHostedExitNodeModel()->index(row, 0);
        if (!index.isValid()) {
            continue;
        }
        const QString exitNodeId = index.data(mRoleIndexId).toString();
        addAction(index.data(mRoleIndexHostName).toString(), this, [this, exitNodeId] {
            mTailscale->preferences()->setExitNodeID(exitNodeId);
        });
    }
}
