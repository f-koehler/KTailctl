#ifndef KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#include <QMenu>

class TrayMenuExitNodes : public QMenu
{
    Q_OBJECT

public:
    explicit TrayMenuExitNodes(QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Exit Nodes"), parent)
    {
        setIcon(QIcon::fromTheme(QStringLiteral("globe")));
    }
};

#endif // KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
