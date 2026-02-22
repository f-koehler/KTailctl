#ifndef KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP

#include "logging_tray_icon.hpp"
#include "tailscale/status/location.hpp"
#include "tailscale/tailscale.hpp"
#include <QMenu>

class TrayMenuExitNodesMullvad : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;
    bool mRoleIndicesFound = false;
    const int mRoleIndexHostName = -1;
    const int mRoleIndexLocation = -1;

    QMap<QString, QMenu *> mPerCountryMenus;

public slots:
    Q_INVOKABLE void rebuildMenu()
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
            const QIcon icon(":/country-flags/country-flag-" + countryCode.toLower());
            if (mPerCountryMenus[countryCode] == nullptr) {
                mPerCountryMenus[countryCode] = new QMenu(location->country(), this);
                mPerCountryMenus[countryCode]->setIcon(icon);
            }
            mPerCountryMenus[countryCode]->addAction(icon, index.data(mRoleIndexHostName).toString());
        }

        for (auto menu : mPerCountryMenus.values()) {
            addMenu(menu);
        }
    }

public:
    explicit TrayMenuExitNodesMullvad(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Mullvad"), parent)
        , mTailscale(tailscale)
        , mRoleIndicesFound(true)
        , mRoleIndexHostName(tailscale->status()->peerModel()->roleIndexForProperty("hostName"))
        , mRoleIndexLocation(tailscale->status()->peerModel()->roleIndexForProperty("location"))
    {
        if (mRoleIndexHostName == -1) {
            qCCritical(Logging::TrayIcon) << "Failed to find role index for hostName";
            mRoleIndicesFound = false;
        }
        if (mRoleIndexLocation == -1) {
            qCCritical(Logging::TrayIcon) << "Failed to find role index for location";
            mRoleIndicesFound = false;
        }
    }
};

#endif // KTAILCTL_TRAY_MENU_MULLVAD_EXIT_NODES_HPP
