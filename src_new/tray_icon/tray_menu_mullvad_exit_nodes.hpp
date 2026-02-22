#ifndef KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP

#include "logging_tray_icon.hpp"
#include "tailscale/tailscale.hpp"
#include <QMenu>

class TrayMenuExitNodesMullvad : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;

public slots:
    Q_INVOKABLE void rebuildMenu()
    {
        clear();
        if (!mRoleIndicesFound) {
            return;
        }
        const int rows = mTailscale->status()->mullvadExitNodeModel()->rowCount(QModelIndex{});
        if (rows <= 0) {
            setEnabled(false);
            return;
        }
        for (int row = 0; row < rows; ++row) {
            QModelIndex index = mTailscale->status()->mullvadExitNodeModel()->index(row, 0);
            if (!index.isValid()) {
                continue;
            }
            QPersistentModelIndex persistent_index(index);
            addAction(index.data(mRoleIndexHostName).toString());
        }
    }

public:
    explicit TrayMenuExitNodesMullvad(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Mullvad"), parent)
        , mTailscale(tailscale)
        , mRoleIndicesFound(true)
        , mRoleIndexHostName((tailscale->status()->peerModel()->roleIndexForProperty("hostName")))
    {
        mRoleIndicesFound = true;
        if (mRoleIndexHostName == -1) {
            qCCritical(Logging::TrayIcon) << "Failed to find role index for hostName";
            mRoleIndicesFound = false;
        }
    }
};

#endif // KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP