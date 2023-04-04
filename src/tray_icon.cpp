#include "tray_icon.h"
#include "util.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>

TrayIcon::TrayIcon(Status *status, QObject *parent)
    : QSystemTrayIcon(parent)
    , mStatus(status)
{
    setContextMenu(new QMenu());

    QObject::connect(contextMenu(), &QMenu::aboutToShow, this, &TrayIcon::regenerate);
    setIcon(QIcon::fromTheme(QStringLiteral("online")));
    show();
}

void TrayIcon::regenerate()
{
    QMenu *menu = contextMenu();
    menu->clear();

    if (mWindow != nullptr) {
        menu->addAction(QIcon::fromTheme("window"), "Open", this, [this]() {
            mWindow->show();
        });
        menu->addSeparator();
    }

    QClipboard *clipboard = QGuiApplication::clipboard();
    auto create_action = [clipboard](QMenu *menu, const QString &text) {
        auto *action = menu->addAction(text);
        connect(action, &QAction::triggered, [clipboard, &text]() {
            clipboard->setText(text);
        });
        return action;
    };
    for (auto peer : mStatus->peers()) {
        auto *submenu = menu->addMenu(loadOsIcon(peer->os()), peer->hostName());
        create_action(submenu, peer->dnsName());
        for (auto address : peer->tailscaleIps()) {
            create_action(submenu, address);
        }
    }

    menu->addSeparator();
    menu->addAction(QIcon::fromTheme("application-exit"), "Quit", this, qApp->quit);
    setContextMenu(menu);
}

void TrayIcon::setWindow(QQuickWindow *window)
{
    mWindow = window;
}