#include "tray_menu_exit_nodes.hpp"

#include <QIcon>
#include <QString>
#include <QStringBuilder>
#include <QStringLiteral>

#include "ktailctl_config.h"
#include "location.hpp"
#include "peer_status.hpp"
#include "preferences.hpp"
#include "status.hpp"
#include "tailscale.hpp"
#include "tray_icon/tray_menu_mullvad_exit_nodes.hpp"
#include "tray_icon/tray_menu_self_hosted_exit_nodes.hpp"

TrayMenuExitNodes::TrayMenuExitNodes(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Exit Nodes"), parent)
    , mTailscale(tailscale)
    , mSelfHosted(new TrayMenuExitNodesSelfHosted(tailscale, this))
    , mMullvad(new TrayMenuExitNodesMullvad(tailscale, this))
{
    setIcon(QIcon::fromTheme(QStringLiteral("globe")));
    mActionSuggested = addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Use suggested"), this, [this] -> void {
        if (const QString suggestedId = mTailscale->status()->suggestedExitNodeId(); !suggestedId.isEmpty()) {
            mTailscale->preferences()->setExitNodeID(suggestedId);
        }
    });
    addSeparator();
    addMenu(mSelfHosted);
    addMenu(mMullvad);
    mActionUnset = addAction(QIcon::fromTheme(QStringLiteral("cancel")), QStringLiteral("Unset"), this, [this] -> void {
        mTailscale->preferences()->setExitNodeID(QString());
    });
    mActionLastUsed = addAction(QIcon::fromTheme(QStringLiteral("document-open-recent")), QStringLiteral("Last used"), this, [this] -> void {
        mTailscale->preferences()->setExitNodeID(Config::lastUsedExitNode());
    });

    connect(this, &QMenu::aboutToShow, this, [this] -> void {
        const QString suggestedId = mTailscale->status()->suggestedExitNodeId();
        const QString exitNodeId = mTailscale->preferences()->exitNodeId();
        const bool hasSuggested = !suggestedId.isEmpty() && suggestedId != exitNodeId;
        mActionSuggested->setVisible(hasSuggested);
        if (hasSuggested) {
            if (const auto *peer = mTailscale->status()->peerWithId(suggestedId); peer) {
                const QString label = peer->dnsName();
                mActionSuggested->setText(QStringLiteral("Use suggested: ") + label);
                if (peer->mullvadNode() && peer->location()) {
                    mActionSuggested->setIcon(QIcon(QStringLiteral(":/country-flags/country-flag-%1").arg(peer->location()->countryCode().toLower())));
                }
            } else {
                mActionSuggested->setText(QStringLiteral("Use suggested: ") + suggestedId);
            }
        }

        const bool hasExitNode = !exitNodeId.isEmpty();
        mActionUnset->setVisible(hasExitNode);

        if (!Config::lastUsedExitNode().isEmpty() && (Config::lastUsedExitNode() != exitNodeId)) {
            mActionLastUsed->setVisible(true);
            mActionLastUsed->setText(QStringLiteral("Last used: ") + mTailscale->status()->peerWithId(Config::lastUsedExitNode())->dnsName());
        } else {
            mActionLastUsed->setVisible(false);
        }
        if (hasExitNode) {
            const auto *peer = mTailscale->status()->peerWithId(exitNodeId);
            const QString label = peer ? peer->dnsName() : exitNodeId;
            mActionUnset->setText(QStringLiteral("Unset: ") + label);
        }
    });
}
