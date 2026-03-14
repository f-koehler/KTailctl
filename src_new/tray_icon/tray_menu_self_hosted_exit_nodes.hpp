#ifndef KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP

#include "logging_tray_icon.hpp"
#include "tailscale/tailscale.hpp"
#include <QMenu>
#include <QObject>

class TrayMenuExitNodesSelfHosted : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexId = -1;

public slots:
    Q_INVOKABLE void rebuildMenu()
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
            const QString id = index.data(mRoleIndexId).toString();
            addAction(index.data(mRoleIndexHostName).toString(), [this, id]() {
                mTailscale->preferences()->setExitNodeID(id);
            });
        }
    }

public:
    explicit TrayMenuExitNodesSelfHosted(TailscaleNew *tailscale, QWidget *parent = nullptr)
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
};

#endif // KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP