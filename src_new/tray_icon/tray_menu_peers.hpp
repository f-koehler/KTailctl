#ifndef KTAILCTL_TRAY_MENU_PEERS_HPP
#define KTAILCTL_TRAY_MENU_PEERS_HPP
#include <QMenu>

class TrayMenuPeers : public QMenu
{
    Q_OBJECT

public:
    explicit TrayMenuPeers(QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Peers"), parent)
    {
        setIcon(QIcon::fromTheme(QStringLiteral("distribute-graph-directed")));
    }
};

#endif // KTAILCTL_TRAY_MENU_PEERS_HPP
