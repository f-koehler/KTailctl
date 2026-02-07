#ifndef KTAILCTL_TRAY_MENU_SELF_HPP
#define KTAILCTL_TRAY_MENU_SELF_HPP
#include <QMenu>

class TrayMenuSelf : public QMenu
{
    Q_OBJECT

private:
    QAction *mActionDnsName;

public:
    explicit TrayMenuSelf(QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Self"), parent)
        , mActionDnsName(new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), QStringLiteral("DnsName")))
    {
        setIcon(QIcon::fromTheme(QStringLiteral("computer")));

        addAction(mActionDnsName);
    }
};

#endif // KTAILCTL_TRAY_MENU_SELF_HPP
