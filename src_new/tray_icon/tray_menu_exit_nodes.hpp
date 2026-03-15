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
            const QString exitNodeId = mTailscale->preferences()->exitNodeId();
            const bool hasExitNode = !exitNodeId.isEmpty();
            mActionUnset->setVisible(hasExitNode);
            if (hasExitNode) {
                const auto *peer = mTailscale->status()->peerWithId(exitNodeId);
                const QString label = peer ? peer->dnsName() : exitNodeId;
                mActionUnset->setText(QStringLiteral("Unset: ") + label);
            }
        });
    }
};

#endif // KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
