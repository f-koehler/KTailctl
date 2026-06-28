#ifndef KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_EXIT_NODES_HPP

#include <QAction>
#include <QMenu>
#include <QWidget>
#include <qtmetamacros.h>

#include "tailscale.hpp"
#include "tray_menu_mullvad_exit_nodes.hpp"
#include "tray_menu_self_hosted_exit_nodes.hpp"

class TrayMenuExitNodes : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    TrayMenuExitNodesSelfHosted *mSelfHosted;
    TrayMenuExitNodesMullvad *mMullvad;
    QAction *mActionSuggested = nullptr;
    QAction *mActionUnset = nullptr;
    QAction *mActionLastUsed = nullptr;

    void updateDynamicActions();

public:
    explicit TrayMenuExitNodes(Tailscale *tailscale, QWidget *parent = nullptr);
};

#endif // KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
