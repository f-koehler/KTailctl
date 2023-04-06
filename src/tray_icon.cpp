#include "tray_icon.h"
#include "util.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>

TrayIcon::TrayIcon(Tailscale *tailscale, QObject *parent)
    : QSystemTrayIcon(parent)
    , mTailscale(tailscale)
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

    auto action_toggle = menu->addAction("Toggle", [this]() {
        mTailscale->toggle();
    });
    action_toggle->setCheckable(true);
    if (mTailscale->status()->backendState() == "Running") {
        action_toggle->setChecked(true);
        action_toggle->setText("Stop Tailscale");
    } else {
        action_toggle->setChecked(false);
        action_toggle->setText("Start Tailscale");
    }
    menu->addSeparator();

    auto create_action = [](QMenu *menu, const QString &text) {
        auto *action = menu->addAction(text);
        connect(action, &QAction::triggered, [text]() {
            setClipboardText(text);
        });
        return action;
    };
    for (auto peer : mTailscale->status()->peers()) {
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