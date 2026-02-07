#ifndef KTAILCTL_TRAY_MENU_SELF_HPP
#define KTAILCTL_TRAY_MENU_SELF_HPP
#include <QMenu>

class TrayMenuSelf : public QMenu
{
    Q_OBJECT

private:
    TailscaleNew mTailscale;

public slots:
    Q_INVOKABLE void rebuildMenu()
    {
        clear();
        const QString &dnsName = mTailscale.status()->self()->dnsName();
        addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), dnsName, [&dnsName]() {
            Util::setClipboardText(dnsName);
        });
        const QStringList &tailscaleIps = mTailscale.status()->self()->tailscaleIps();
        for (const QString &ip : tailscaleIps) {
            addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), ip, [&ip]() {
                Util::setClipboardText(ip);
            });
        }
    }

public:
    explicit TrayMenuSelf(TailscaleNew *tailscale, QWidget *parent = nullptr)
        : QMenu(QStringLiteral("Self"), parent)
        , mTailscale(tailscale)
    {
        setIcon(QIcon::fromTheme(QStringLiteral("computer")));
    }
};

#endif // KTAILCTL_TRAY_MENU_SELF_HPP
