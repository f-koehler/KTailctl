#ifndef KTAILCTL_TRAY_MENU_ACCOUNTS_HPP
#define KTAILCTL_TRAY_MENU_ACCOUNTS_HPP
#include "logging_tray_icon.hpp"
#include "tailscale.hpp"
#include <QMenu>
#include <QModelIndex>

class TrayMenuAccounts : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    int mRoleIndexName = -1;

public:
    explicit TrayMenuAccounts(Tailscale *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Accounts"), parent)
        , mTailscale(tailscale)
        , mRoleIndexName(tailscale->loginProfileModel()->roleIndexForProperty("name"))
    {
        setIcon(QIcon::fromTheme(QStringLiteral("config-users")));
        if (mRoleIndexName == -1) {
            qCCritical(Logging::TrayIcon) << "Failed to find role index for name";
            mRoleIndicesFound = false;
        }

        connect(this, &QMenu::aboutToShow, this, &TrayMenuAccounts::rebuildMenu);
    }

public Q_SLOTS:
    void rebuildMenu()
    {
        clear();
        if (!mRoleIndicesFound) {
            return;
        }
        const int rows = mTailscale->loginProfileModel()->rowCount(QModelIndex{});
        for (int row = 0; row < rows; ++row) {
            const QModelIndex index = mTailscale->loginProfileModel()->index(row, 0);
            QAction *action = addAction(mTailscale->loginProfileModel()->data(index, mRoleIndexName).toString());
            action->setCheckable(true);
        }
    }
};

#endif // KTAILCTL_TRAY_MENU_ACCOUNTS_HPP
