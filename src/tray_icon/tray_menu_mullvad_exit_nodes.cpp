#include "tray_menu_mullvad_exit_nodes.hpp"

#include <QByteArray>
#include <QIcon>
#include <QMessageLogger>
#include <QModelIndex>
#include <QStringBuilder>
#include <QStringLiteral>
#include <QVariant>
#include <QtCore>
#include <utility>

#include "location.hpp"
#include "logging_tray_icon.hpp"
#include "mullvad_exit_node_model.hpp"
#include "preferences.hpp"
#include "status.hpp"
#include "tailscale.hpp"

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
    connect(this, &QMenu::aboutToShow, this, &TrayMenuExitNodesMullvad::rebuildMenu);
}

void TrayMenuExitNodesMullvad::rebuildMenu()
{
    clear();
    for (auto [key, value] : mPerCountryMenus.asKeyValueRange()) {
        value->clear();
    }

    if (!mRoleIndicesFound) {
        return;
    }
    const int rows = mTailscale->status()->mullvadExitNodeModel()->rowCount(QModelIndex{});
    if (rows <= 0) {
        setEnabled(false);
        return;
    }

    for (int row = 0; row < rows; ++row) {
        const QModelIndex index = mTailscale->status()->mullvadExitNodeModel()->index(row, 0);
        if (!index.isValid()) {
            continue;
        }
        const auto *location = index.data(mRoleIndexLocation).value<Location *>();
        if (location == nullptr) {
            continue;
        }
        const auto &countryCode = location->countryCode();
        const QIcon icon(QStringLiteral(":/country-flags/country-flag-") + countryCode.toLower());
        auto menuIt = mPerCountryMenus.find(countryCode);
        if (menuIt == mPerCountryMenus.end()) {
            auto *newMenu = new QMenu(location->country(), this);
            newMenu->setIcon(icon);
            menuIt = mPerCountryMenus.insert(countryCode, newMenu);
        }
        const auto exitNodeId = index.data(mRoleIndexId).value<QString>();
        menuIt.value()->addAction(icon, index.data(mRoleIndexHostName).toString(), this, [this, exitNodeId] -> void {
            mTailscale->preferences()->setExitNodeID(exitNodeId);
        });
    }

    for (auto *menu : std::as_const(mPerCountryMenus)) {
        addMenu(menu);
    }
}
