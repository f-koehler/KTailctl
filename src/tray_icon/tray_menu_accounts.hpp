#ifndef KTAILCTL_TRAY_MENU_ACCOUNTS_HPP
#define KTAILCTL_TRAY_MENU_ACCOUNTS_HPP
#include <QMenu>

class TrayMenuAccounts : public QMenu
{
    Q_OBJECT

public:
    explicit TrayMenuAccounts(QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Accounts"), parent)
    {
        setIcon(QIcon::fromTheme(QStringLiteral("config-users")));
    }
};

#endif // KTAILCTL_TRAY_MENU_ACCOUNTS_HPP
