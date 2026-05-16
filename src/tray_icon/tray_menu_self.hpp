#ifndef KTAILCTL_TRAY_MENU_SELF_HPP
#define KTAILCTL_TRAY_MENU_SELF_HPP

#include <QIcon>
#include <QMenu>
#include <QString>
#include <QStringLiteral>
#include <QWidget>
#include <qtmetamacros.h>

#include "tailscale/tailscale.hpp"

class TrayMenuSelf : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;

public:
    explicit TrayMenuSelf(Tailscale *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Self"), parent)
        , mTailscale(tailscale)
    {
        setIcon(QIcon::fromTheme(QStringLiteral("computer")));
        connect(this, &QMenu::aboutToShow, this, &TrayMenuSelf::rebuildMenu);
    }

    Q_SLOT Q_INVOKABLE void rebuildMenu();
};

#endif // KTAILCTL_TRAY_MENU_SELF_HPP
