#ifndef KTAILCTL_TRAY_MENU_ACCOUNTS_HPP
#define KTAILCTL_TRAY_MENU_ACCOUNTS_HPP

#include "tailscale/tailscale.hpp"
#include <QIcon>
#include <QMap>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QString>

class TrayMenuAccounts : public QMenu
{
    Q_OBJECT

    Tailscale *mTailscale;
    bool mRoleIndicesFound = false;
    QString mLastSignature;
    bool mBuilt = false;
    const int mRoleIndexId = -1;
    const int mRoleIndexName = -1;
    QNetworkAccessManager *mNetworkManager;
    QMap<QString, QIcon> mAvatarCache;

    void fetchAvatar(const QString &url);
    QIcon resolveIcon(const QString &picUrl);

public:
    explicit TrayMenuAccounts(Tailscale *tailscale, QWidget *parent = nullptr);

Q_SIGNALS:
    void avatarLoaded();

public Q_SLOTS:
    Q_INVOKABLE void rebuildMenu();
};

#endif // KTAILCTL_TRAY_MENU_ACCOUNTS_HPP
