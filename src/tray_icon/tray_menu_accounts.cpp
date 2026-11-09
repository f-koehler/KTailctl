#include "tray_menu_accounts.hpp"
#include "logging_tray_icon.hpp"
#include <QIcon>
#include <QModelIndex>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>

#include "tray_icon/tray_menu_util.hpp"

TrayMenuAccounts::TrayMenuAccounts(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Accounts"), parent)
    , mTailscale(tailscale)
    , mRoleIndicesFound(true)
    , mRoleIndexId(tailscale->loginProfileModel()->roleIndexForProperty("id"))
    , mRoleIndexName(tailscale->loginProfileModel()->roleIndexForProperty("name"))
    , mNetworkManager(new QNetworkAccessManager(this))
{
    setIcon(QIcon::fromTheme(QStringLiteral("system-users")));
    if (mRoleIndexId == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for id";
        mRoleIndicesFound = false;
    }
    if (mRoleIndexName == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for name";
        mRoleIndicesFound = false;
    }
    QTimer *timer = TrayMenuUtil::makeRebuildTimer(this, [this] -> void {
        rebuildMenu();
    });
    TrayMenuUtil::rebuildOnModelChange(mTailscale->loginProfileModel(), timer);
    connect(mTailscale, &Tailscale::currentLoginProfileIdChanged, timer, [timer] -> void {
        timer->start();
    });
    connect(this, &TrayMenuAccounts::avatarLoaded, timer, [timer] -> void {
        timer->start();
    });
    rebuildMenu();
}

void TrayMenuAccounts::fetchAvatar(const QString &url)
{
    if (url.isEmpty() || mAvatarCache.contains(url)) {
        return;
    }
    mAvatarCache[url] = QIcon::fromTheme(QStringLiteral("user"));
    auto *reply = mNetworkManager->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [this, url, reply] {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            return;
        }
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        if (!pixmap.isNull()) {
            mAvatarCache[url] = QIcon(pixmap);
            Q_EMIT avatarLoaded();
        }
    });
}

QIcon TrayMenuAccounts::resolveIcon(const QString &picUrl)
{
    if (picUrl.isEmpty()) {
        return QIcon::fromTheme(QStringLiteral("user"));
    }
    if (mAvatarCache.contains(picUrl)) {
        return mAvatarCache[picUrl];
    }
    fetchAvatar(picUrl);
    return QIcon::fromTheme(QStringLiteral("user"));
}

void TrayMenuAccounts::rebuildMenu()
{
    if (!mRoleIndicesFound) {
        return;
    }

    struct Account {
        QString id;
        QString name;
        QIcon icon;
    };
    QList<Account> accounts;
    QString signature;
    const QString currentId = mTailscale->currentLoginProfileId();
    signature += currentId + QLatin1Char('\x1e');
    const int rows = mTailscale->loginProfileModel()->rowCount(QModelIndex{});
    for (int row = 0; row < rows; ++row) {
        const QModelIndex index = mTailscale->loginProfileModel()->index(row, 0);
        QString profileId = mTailscale->loginProfileModel()->data(index, mRoleIndexId).toString();
        QString name = mTailscale->loginProfileModel()->data(index, mRoleIndexName).toString();

        QIcon icon = QIcon::fromTheme(QStringLiteral("user"));
        if (const LoginProfile *profile = mTailscale->loginProfileWithId(profileId)) {
            icon = resolveIcon(profile->userProfile()->profilePicUrl());
        }
        signature += profileId + QLatin1Char('\x1f') + name + QLatin1Char('\x1f') + QString::number(icon.cacheKey()) + QLatin1Char('\x1e');
        accounts.append({.id = std::move(profileId), .name = std::move(name), .icon = std::move(icon)});
    }
    if (mBuilt && signature == mLastSignature) {
        return;
    }
    mBuilt = true;
    mLastSignature = signature;

    clear();
    if (accounts.isEmpty()) {
        addAction(tr("No accounts"))->setEnabled(false);
        return;
    }
    for (const Account &account : std::as_const(accounts)) {
        QAction *action = addAction(account.icon, account.name);
        action->setCheckable(true);
        action->setChecked(account.id == currentId);
        if (account.id != currentId) {
            const QString profileId = account.id;
            connect(action, &QAction::triggered, this, [this, profileId] {
                mTailscale->switchAccount(profileId);
            });
        }
    }
}
