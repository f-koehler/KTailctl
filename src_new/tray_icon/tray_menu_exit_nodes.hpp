#ifndef KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#define KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
#include <QMenu>

class TrayMenuExitNodesMullvad : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;

public slots:
    Q_INVOKABLE void rebuildMenu()
    {
    }

public:
    explicit TrayMenuExitNodesMullvad(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Exit Nodes"), parent)
        , mTailscale(tailscale)
    {
    }
};

class TrayMenuExitNodesSelfHosted : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;

public slots:
    Q_INVOKABLE void rebuildMenu()
    {
    }

public:
    explicit TrayMenuExitNodesSelfHosted(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Self-Hosted"), parent)
        , mTailscale(tailscale)
    {
    }
};

class TrayMenuExitNodes : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew *mTailscale;
    TrayMenuExitNodesMullvad *mSelfHosted;
    TrayMenuExitNodesSelfHosted *mMullvad;

public slots:
    Q_INVOKABLE void rebuildMenu()
    {
        mSelfHosted->rebuildMenu();
        mMullvad->rebuildMenu();
    }

public:
    explicit TrayMenuExitNodes(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Exit Nodes"), parent)
        , mTailscale(tailscale), mSelfHosted()
    {
        setIcon(QIcon::fromTheme(QStringLiteral("globe")));

        // mSelfHosted = addMenu(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Self-Hosted"));
        // mMullvad = addMenu(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Mullvad"));
    }
};

#endif // KTAILCTL_TRAY_MENU_EXIT_NODES_HPP
