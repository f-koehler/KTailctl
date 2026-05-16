#ifndef KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP

#include <QMap>
#include <QMenu>
#include <QString>
#include <QWidget>
#include <qtmetamacros.h>

#include "tailscale/tailscale.hpp"

class TrayMenuExitNodesMullvad : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexLocation = -1;
    const int mRoleIndexId = -1;

    QMap<QString, QMenu *> mPerCountryMenus;

public:
    explicit TrayMenuExitNodesMullvad(Tailscale *tailscale, QWidget *parent = nullptr);

    Q_INVOKABLE void rebuildMenu();
};

#endif // KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP
