#include "tray_menu_mullvad_exit_nodes.hpp"

#include <QByteArray>
#include <QIcon>
#include <QList>
#include <QMessageLogger>
#include <QModelIndex>
#include <QStringBuilder>
#include <QStringLiteral>
#include <QVariant>
#include <Qt>
#include <QtAlgorithms>
#include <utility>

#include "location.hpp"
#include "logging_tray_icon.hpp"
#include "mullvad_exit_node_model.hpp"
#include "preferences.hpp"
#include "status.hpp"
#include "tailscale.hpp"
#include "tray_icon/tray_menu_util.hpp"

TrayMenuExitNodesMullvad::TrayMenuExitNodesMullvad(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Mullvad"), parent)
    , mTailscale(tailscale)
    , mRoleIndicesFound(true)
    , mRoleIndexHostName(tailscale->status()->peerModel()->roleIndexForProperty("hostName"))
    , mRoleIndexLocation(tailscale->status()->peerModel()->roleIndexForProperty("location"))
    , mRoleIndexId(tailscale->status()->peerModel()->roleIndexForProperty("id"))
{
    setIcon(QIcon(QStringLiteral(":/icons/mullvad.svg")));
    if (mRoleIndexHostName == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for hostName";
        mRoleIndicesFound = false;
    }
    if (mRoleIndexLocation == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for location";
        mRoleIndicesFound = false;
    }
    if (mRoleIndexId == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for id";
        mRoleIndicesFound = false;
    }
    QTimer *timer = TrayMenuUtil::makeRebuildTimer(this, [this] -> void {
        rebuildMenu();
    });
    TrayMenuUtil::rebuildOnModelChange(mTailscale->status()->mullvadExitNodeModel(), timer);
    rebuildMenu();
}

void TrayMenuExitNodesMullvad::rebuildMenu()
{
    if (!mRoleIndicesFound) {
        return;
    }

    struct Node {
        QString id;
        QString hostName;
        QString countryCode;
        QString country;
    };
    QList<Node> nodes;
    QString signature;
    const int rows = mTailscale->status()->mullvadExitNodeModel()->rowCount(QModelIndex{});
    for (int row = 0; row < rows; ++row) {
        const QModelIndex index = mTailscale->status()->mullvadExitNodeModel()->index(row, 0);
        if (!index.isValid()) {
            continue;
        }
        const auto *location = index.data(mRoleIndexLocation).value<Location *>();
        if (location == nullptr) {
            continue;
        }
        Node node{.id = index.data(mRoleIndexId).value<QString>(),
                  .hostName = index.data(mRoleIndexHostName).toString(),
                  .countryCode = location->countryCode(),
                  .country = location->country()};
        signature +=
            node.id + QLatin1Char('\x1f') + node.hostName + QLatin1Char('\x1f') + node.countryCode + QLatin1Char('\x1f') + node.country + QLatin1Char('\x1e');
        nodes.append(std::move(node));
    }
    if (mBuilt && signature == mLastSignature) {
        return;
    }
    mBuilt = true;
    mLastSignature = signature;

    clear();
    qDeleteAll(mPerCountryMenus);
    mPerCountryMenus.clear();

    setEnabled(!nodes.isEmpty());
    if (nodes.isEmpty()) {
        addAction(tr("No Mullvad exit nodes"))->setEnabled(false);
        return;
    }

    for (const Node &node : std::as_const(nodes)) {
        const QIcon icon(QStringLiteral(":/country-flags/country-flag-") + node.countryCode.toLower());
        auto menuIt = mPerCountryMenus.find(node.countryCode);
        if (menuIt == mPerCountryMenus.end()) {
            auto *newMenu = new QMenu(node.country, this);
            newMenu->setIcon(icon);
            menuIt = mPerCountryMenus.insert(node.countryCode, newMenu);
        }
        const QString exitNodeId = node.id;
        menuIt.value()->addAction(icon, node.hostName, this, [this, exitNodeId] -> void {
            mTailscale->preferences()->setExitNodeID(exitNodeId);
        });
    }

    // Country submenus are fully populated before they are added (and thereby exported)
    for (auto *menu : std::as_const(mPerCountryMenus)) {
        addMenu(menu);
    }
}
