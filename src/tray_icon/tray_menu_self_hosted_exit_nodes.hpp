#ifndef KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP

#include <QMenu>
#include <QWidget>
#include <qtmetamacros.h>

#include "tailscale/tailscale.hpp"

class TrayMenuExitNodesSelfHosted : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexId = -1;

public:
    explicit TrayMenuExitNodesSelfHosted(Tailscale *tailscale, QWidget *parent = nullptr);

    Q_INVOKABLE void rebuildMenu();
};

#endif // KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP
