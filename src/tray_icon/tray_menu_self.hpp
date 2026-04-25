#ifndef KTAILCTL_TRAY_MENU_SELF_HPP
#define KTAILCTL_TRAY_MENU_SELF_HPP

#include "tailscale/tailscale.hpp"
#include <QMenu>

class TrayMenuSelf : public QMenu
{
    Q_OBJECT

private:
    Tailscale *mTailscale;

public Q_SLOTS:
    Q_INVOKABLE void rebuildMenu();

public:
    explicit TrayMenuSelf(Tailscale *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Self"), parent)
        , mTailscale(tailscale)
    {
        setIcon(QIcon::fromTheme(QStringLiteral("computer")));
        connect(this, &QMenu::aboutToShow, this, &TrayMenuSelf::rebuildMenu);
    }
};

#endif // KTAILCTL_TRAY_MENU_SELF_HPP
