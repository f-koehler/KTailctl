#include "tray_menu_exit_nodes.hpp"

TrayMenuExitNodes::TrayMenuExitNodes(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Exit Nodes"), parent)
    , mTailscale(tailscale)
    , mSelfHosted(new TrayMenuExitNodesSelfHosted(tailscale, this))
    , mMullvad(new TrayMenuExitNodesMullvad(tailscale, this))
{
    setIcon(QIcon::fromTheme(QStringLiteral("globe")));
    mActionSuggested = addAction(QIcon::fromTheme("network-vpn"), "Use suggested", [this]() {
        const QString suggestedId = mTailscale->status()->suggestedExitNodeId();
        if (!suggestedId.isEmpty()) {
            mTailscale->preferences()->setExitNodeID(suggestedId);
        }
    });
    addSeparator();
    addMenu(mSelfHosted);
    addMenu(mMullvad);
    mActionUnset = addAction(QIcon::fromTheme("cancel"), "Unset", [this]() {
        mTailscale->preferences()->setExitNodeID(QString());
    });
    mActionLastUsed = addAction(QIcon::fromTheme("document-open-recent"), "Last used", [this]() {
        mTailscale->preferences()->setExitNodeID(Config::self()->lastUsedExitNode());
    });

    connect(this, &QMenu::aboutToShow, this, [this]() {
        const QString suggestedId = mTailscale->status()->suggestedExitNodeId();
        const QString exitNodeId = mTailscale->preferences()->exitNodeId();
        const bool hasSuggested = !suggestedId.isEmpty() && suggestedId != exitNodeId;
        mActionSuggested->setVisible(hasSuggested);
        if (hasSuggested) {
            const auto *peer = mTailscale->status()->peerWithId(suggestedId);
            if (peer) {
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
            mActionLastUsed->setText("Last used: " + mTailscale->status()->peerWithId(Config::lastUsedExitNode())->dnsName());
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
