#ifndef KTAILCTL_TRAY_ICON_HPP
#define KTAILCTL_TRAY_ICON_HPP

#include "./tray_menu_accounts.hpp"
#include "./tray_menu_exit_nodes.hpp"
#include "./tray_menu_peers.hpp"
#include "./tray_menu_self.hpp"
#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

private:
    QMenu *mContextMenu;
    TrayMenuAccounts *mMenuAccounts;
    TrayMenuSelf *mMenuSelf;
    TrayMenuPeers *mMenuPeers;
    TrayMenuExitNodes *mMenuExitNodes;
    QAction *mActionShow;
    QAction *mActionToggle;
    QAction *mActionQuit;

signals:
    void showWindow();
    void quitRequested();
    void toggleTailscale();

public:
    explicit TrayIcon(TailscaleNew *tailscale, QObject *parent = nullptr)
        : QSystemTrayIcon(QIcon::fromTheme(QStringLiteral("about_kde")), parent)
        , mContextMenu(new QMenu(QStringLiteral("Tray Menu")))
        , mMenuAccounts(new TrayMenuAccounts())
        , mMenuSelf(new TrayMenuSelf(tailscale))
        , mMenuPeers(new TrayMenuPeers(tailscale))
        , mMenuExitNodes(new TrayMenuExitNodes(tailscale))
        , mActionShow(new QAction(QIcon::fromTheme(QStringLiteral("window")), QStringLiteral("Show")))
        , mActionToggle(new QAction(QIcon::fromTheme(QStringLiteral("stop")), QStringLiteral("Stop")))
        , mActionQuit(new QAction(QIcon::fromTheme(QStringLiteral("application-exit")), QStringLiteral("Quit")))
    {
        setContextMenu(mContextMenu);
        mContextMenu->addAction(mActionShow);
        mContextMenu->addSeparator();
        mContextMenu->addMenu(mMenuAccounts);
        mContextMenu->addMenu(mMenuSelf);
        mContextMenu->addMenu(mMenuPeers);
        mContextMenu->addMenu(mMenuExitNodes);
        mContextMenu->addAction(mActionToggle);
        mContextMenu->addSeparator();
        mContextMenu->addAction(mActionQuit);

        connect(mActionShow, &QAction::triggered, this, [this] {
            emit showWindow();
        });
        connect(mActionToggle, &QAction::triggered, this, [this] {
            emit toggleTailscale();
        });
        connect(mActionQuit, &QAction::triggered, this, [this] {
            emit quitRequested();
        });

        connect(mContextMenu, &QMenu::aboutToShow, mMenuSelf, &TrayMenuSelf::rebuildMenu);
        connect(mContextMenu, &QMenu::aboutToShow, mMenuPeers, &TrayMenuPeers::rebuildMenu);
        connect(mContextMenu, &QMenu::aboutToShow, mMenuExitNodes, &TrayMenuExitNodes::rebuildMenu);
    }
};

#endif // KTAILCTL_TRAY_ICON_HPP
