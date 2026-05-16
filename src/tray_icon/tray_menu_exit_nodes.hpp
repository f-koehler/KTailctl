#ifndef KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_EXIT_NODES_HPP

#include "tray_menu_mullvad_exit_nodes.hpp"
#include "tray_menu_self_hosted_exit_nodes.hpp"
#include <QMenu>

class TrayMenuExitNodes : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    TrayMenuExitNodesSelfHosted *mSelfHosted;
    TrayMenuExitNodesMullvad *mMullvad;
    QAction *mActionSuggested = nullptr;
    QAction *mActionUnset = nullptr;
    QAction *mActionLastUsed = nullptr;

public:
    explicit TrayMenuExitNodes(Tailscale *tailscale, QWidget *parent = nullptr);
};

#endif // KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
