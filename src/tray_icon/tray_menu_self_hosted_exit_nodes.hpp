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
    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexId = -1;

public slots:
    Q_INVOKABLE void rebuildMenu();

public:
    explicit TrayMenuExitNodesSelfHosted(Tailscale *tailscale, QWidget *parent = nullptr);
};

#endif // KTAILCTL_TRAY_MENU_SELF_HOSTED_EXIT_NODES_HPP