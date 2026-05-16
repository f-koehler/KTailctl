#include "tray_icon.hpp"

#include <QChar>
#include <QIcon>
#include <QLatin1Char>
#include <QList>
#include <QString>
#include <QStringList>
#include <QStringLiteral>
#include <qtmetamacros.h>

#include "exit_node_status.hpp"
#include "ktailctl_config.h"
#include "peer_status.hpp"
#include "status.hpp"
#include "tailscale.hpp"
#include "tray_icon/tray_menu_accounts.hpp"
#include "tray_icon/tray_menu_exit_nodes.hpp"
#include "tray_icon/tray_menu_peers.hpp"
#include "tray_icon/tray_menu_self.hpp"

TrayIcon::TrayIcon(Tailscale *tailscale, QObject *parent)
    : QSystemTrayIcon(QIcon::fromTheme(QStringLiteral("unknown")), parent)
    , mTailscale(tailscale)
    , mContextMenu(new QMenu(QStringLiteral("Tray Menu")))
    , mMenuAccounts(new TrayMenuAccounts(tailscale))
    , mMenuSelf(new TrayMenuSelf(tailscale))
    , mMenuPeers(new TrayMenuPeers(tailscale))
    , mMenuExitNodes(new TrayMenuExitNodes(tailscale))
    , mActionShow(new QAction(QIcon::fromTheme(QStringLiteral("window")), QStringLiteral("Show")))
    , mActionStart(new QAction(QIcon::fromTheme(QStringLiteral("media-playback-start")), QStringLiteral("Start")))
    , mActionStop(new QAction(QIcon::fromTheme(QStringLiteral("process-stop")), QStringLiteral("Stop")))
    , mActionQuit(new QAction(QIcon::fromTheme(QStringLiteral("application-exit")), QStringLiteral("Quit")))
{
    setContextMenu(mContextMenu);
    mContextMenu->addAction(mActionShow);
    mContextMenu->addSeparator();
    mContextMenu->addMenu(mMenuAccounts);
    mContextMenu->addMenu(mMenuSelf);
    mContextMenu->addMenu(mMenuPeers);
    mContextMenu->addMenu(mMenuExitNodes);
    mContextMenu->addAction(mActionStart);
    mContextMenu->addAction(mActionStop);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mActionQuit);

    connect(mActionShow, &QAction::triggered, this, [this] -> void {
        Q_EMIT showWindow();
    });
    connect(mActionStart, &QAction::triggered, this, [this] -> void {
        mTailscale->up();
    });
    connect(mActionStop, &QAction::triggered, this, [this] -> void {
        mTailscale->down();
    });
    connect(mActionQuit, &QAction::triggered, this, [this] -> void {
        Q_EMIT quitRequested();
    });

    connect(mContextMenu, &QMenu::aboutToShow, this, [this] -> void {
        if ((mTailscale->status()->backendState() == Status::BackendState::Starting)
            || (mTailscale->status()->backendState() == Status::BackendState::Running)) {
            mActionStart->setVisible(false);
            mActionStop->setVisible(true);
        } else {
            mActionStart->setVisible(true);
            mActionStop->setVisible(false);
        }
    });

    connect(Config::self(), &Config::enableTrayIconChanged, this, [this] -> void {
        setVisible(Config::enableTrayIcon());
    });

    connect(Config::self(), &Config::trayIconThemeChanged, this, &TrayIcon::updateIcon);
    connect(mTailscale->status(), &Status::backendStateChanged, this, &TrayIcon::updateIcon);
    connect(mTailscale->status(), &Status::exitNodeStatusChanged, this, &TrayIcon::updateIcon);

    connect(mTailscale->status(), &Status::backendStateChanged, this, &TrayIcon::updateTooltip);
    connect(mTailscale->status(), &Status::exitNodeStatusChanged, this, &TrayIcon::updateTooltip);

    updateIcon();
    updateTooltip();
}

namespace
{
auto backendStateToString(Status::BackendState state) -> QString
{
    switch (state) {
    case Status::BackendState::NoState:
        return QStringLiteral("No State");
    case Status::BackendState::NeedsLogin:
        return QStringLiteral("Needs Login");
    case Status::BackendState::NeedsMachineAuth:
        return QStringLiteral("Needs Machine Auth");
    case Status::BackendState::Stopped:
        return QStringLiteral("Stopped");
    case Status::BackendState::Starting:
        return QStringLiteral("Starting");
    case Status::BackendState::Running:
        return QStringLiteral("Running");
    }
    return QStringLiteral("Unknown");
}
} // anonymous namespace

void TrayIcon::updateTooltip()
{
    QStringList tooltip;
    tooltip << QStringLiteral("Status: %1").arg(backendStateToString(mTailscale->status()->backendState()));

    if (mTailscale->status()->self() != nullptr && !mTailscale->status()->self()->tailscaleIps().empty()) {
        tooltip << QStringLiteral("IP: %1").arg(mTailscale->status()->self()->tailscaleIps().front());
    }

    if (mTailscale->status()->exitNodeStatus() != nullptr) {
        const auto *exitNode = mTailscale->status()->peerWithId(mTailscale->status()->exitNodeStatus()->id());
        if (exitNode != nullptr && !exitNode->dnsName().isEmpty()) {
            tooltip << QStringLiteral("Exit node: %1").arg(exitNode->dnsName());
        }
    }

    setToolTip(tooltip.join(QLatin1Char('\n')));
}

void TrayIcon::updateIcon()
{
    if ((mTailscale->status()->backendState() == Status::BackendState::Starting) || (mTailscale->status()->backendState() == Status::BackendState::Running)) {
        if (mTailscale->status()->exitNodeStatus() != nullptr) {
            setIcon(QIcon(QStringLiteral(":/tray-icons/exit-node-%1.svg").arg(Config::trayIconTheme())));
        } else {
            setIcon(QIcon(QStringLiteral(":/tray-icons/online-%1.svg").arg(Config::trayIconTheme())));
        }
    } else {
        setIcon(QIcon(QStringLiteral(":/tray-icons/offline-%1.svg").arg(Config::trayIconTheme())));
    }
}
