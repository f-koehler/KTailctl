#ifndef KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP

#include "logging_tray_icon.hpp"
#include "tailscale/status/location.hpp"
#include "tailscale/tailscale.hpp"
#include <QMenu>

class TrayMenuExitNodesMullvad : public QMenu
{
    Q_OBJECT

private:
    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexLocation = -1;
    const int mRoleIndexId = -1;

    QMap<QString, QMenu *> mPerCountryMenus;

public slots:
    Q_INVOKABLE void rebuildMenu();

public:
    explicit TrayMenuExitNodesMullvad(Tailscale *tailscale, QWidget *parent = nullptr);
};

#endif // KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP
