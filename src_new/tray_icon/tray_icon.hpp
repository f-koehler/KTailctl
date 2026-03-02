#ifndef KTAILCTL_TRAY_ICON_HPP
#define KTAILCTL_TRAY_ICON_HPP

#include "./tray_menu_accounts.hpp"
#include "./tray_menu_exit_nodes.hpp"
#include "./tray_menu_peers.hpp"
#include "./tray_menu_self.hpp"
#include "ktailctl_config.h"
#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;
    QMenu *mContextMenu;
    TrayMenuAccounts *mMenuAccounts;
    TrayMenuSelf *mMenuSelf;
    TrayMenuPeers *mMenuPeers;
    TrayMenuExitNodes *mMenuExitNodes;
    QAction *mActionShow;
    QAction *mActionStart;
    QAction *mActionStop;
    QAction *mActionQuit;

signals:
    void showWindow();
    void quitRequested();

public slots:
    Q_INVOKABLE void updateIcon()
    {
        if ((mTailscale->status()->backendState() == Status::BackendState::Starting)
            || (mTailscale->status()->backendState() == Status::BackendState::Running)) {
            setIcon(QIcon(QString(":/tray-icons/online-%1.svg").arg(Config::trayIconTheme())));
        } else {
            setIcon(QIcon(QString(":/tray-icons/offline-%1.svg").arg(Config::trayIconTheme())));
        }
    }

public:
    explicit TrayIcon(TailscaleNew *tailscale, QObject *parent = nullptr)
        : QSystemTrayIcon(QIcon::fromTheme(QStringLiteral("unknown")), parent)
        , mTailscale(tailscale)
        , mContextMenu(new QMenu(QStringLiteral("Tray Menu")))
        , mMenuAccounts(new TrayMenuAccounts())
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

        connect(mActionShow, &QAction::triggered, this, [this] {
            emit showWindow();
        });
        connect(mActionStart, &QAction::triggered, this, [this] {
            mTailscale->up();
        });
        connect(mActionStop, &QAction::triggered, this, [this] {
            mTailscale->down();
        });
        connect(mActionQuit, &QAction::triggered, this, [this] {
            emit quitRequested();
        });

        connect(mContextMenu, &QMenu::aboutToShow, [this]() {
            if ((mTailscale->status()->backendState() == Status::BackendState::Starting)
                || (mTailscale->status()->backendState() == Status::BackendState::Running)) {
                mActionStart->setVisible(false);
                mActionStop->setVisible(true);
            } else {
                mActionStart->setVisible(true);
                mActionStop->setVisible(false);
            }
        });

        connect(Config::self(), &Config::enableTrayIconChanged, [this]() {
            setVisible(Config::enableTrayIcon());
        });

        connect(Config::self(), &Config::trayIconThemeChanged, this, &TrayIcon::updateIcon);
        connect(mTailscale->status(), &Status::backendStateChanged, this, &TrayIcon::updateIcon);
        updateIcon();
    }
};

#endif // KTAILCTL_TRAY_ICON_HPP
