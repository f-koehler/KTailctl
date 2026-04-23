#ifndef KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_EXIT_NODES_HPP

#include "ktailctl_config.h"
#include "tray_menu_mullvad_exit_nodes.hpp"
#include "tray_menu_self_hosted_exit_nodes.hpp"
#include <QMenu>

class TrayMenuExitNodes : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;
    TrayMenuExitNodesSelfHosted *mSelfHosted;
    TrayMenuExitNodesMullvad *mMullvad;
    QAction *mActionSuggested = nullptr;
    QAction *mActionUnset = nullptr;
    QAction *mActionLastUsed = nullptr;

public:
    explicit TrayMenuExitNodes(TailscaleNew *tailscale, QWidget *parent = nullptr);
};

#endif // KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
