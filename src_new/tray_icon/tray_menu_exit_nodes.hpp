#ifndef KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#include <QMenu>

#include "logging_tray_icon.hpp"

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

class TrayMenuExitNodesSelfHosted : public QMenu
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
        const int rows = mTailscale->status()->selfHostedExitNodeModel()->rowCount(QModelIndex{});
        if (rows <= 0) {
            setEnabled(false);
            return;
        }
        for (int row = 0; row < rows; ++row) {
            QModelIndex index = mTailscale->status()->selfHostedExitNodeModel()->index(row, 0);
            if (!index.isValid()) {
                continue;
            }
            QPersistentModelIndex persistent_index(index);
            addAction(index.data(mRoleIndexHostName).toString());
        }
    }

public:
    explicit TrayMenuExitNodesSelfHosted(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Self-Hosted"), parent)
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

class TrayMenuExitNodes : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;
    TrayMenuExitNodesSelfHosted *mSelfHosted;
    TrayMenuExitNodesMullvad *mMullvad;

public slots:
    Q_INVOKABLE void rebuildMenu()
    {
        mSelfHosted->rebuildMenu();
        mMullvad->rebuildMenu();
    }

public:
    explicit TrayMenuExitNodes(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Exit Nodes"), parent)
        , mTailscale(tailscale)
        , mSelfHosted(new TrayMenuExitNodesSelfHosted(tailscale, this))
        , mMullvad(new TrayMenuExitNodesMullvad(tailscale, this))
    {
        setIcon(QIcon::fromTheme(QStringLiteral("globe")));
        addMenu(mSelfHosted);
        addMenu(mMullvad);
    }
};

#endif // KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
