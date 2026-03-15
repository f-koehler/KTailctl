#ifndef KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_EXIT_NODES_HPP

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
    QAction *mActionUnset = nullptr;

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
        mActionUnset = addAction(QIcon::fromTheme("cancel"), "Unset", [this]() {
            mTailscale->preferences()->setExitNodeID(QString());
        });

        connect(this, &QMenu::aboutToShow, this, [this]() {
            mActionUnset->setVisible(!mTailscale->preferences()->exitNodeId().isEmpty());
        });
    }
};

#endif // KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
