#ifndef KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP

#include "logging_tray_icon.hpp"
#include "tailscale/tailscale.hpp"
#include <QMenu>
#include <QObject>

class TrayMenuExitNodesSelfHosted : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexId = -1;

public:
    explicit TrayMenuExitNodesSelfHosted(Tailscale *tailscale, QWidget *parent = nullptr);

public Q_SLOTS:
    Q_INVOKABLE void rebuildMenu();
};

#endif // KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP