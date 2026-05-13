#include "tray_menu_accounts.hpp"
#include "logging_tray_icon.hpp"
#include <QIcon>
#include <QModelIndex>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>

TrayMenuAccounts::TrayMenuAccounts(Tailscale *tailscale, QWidget *parent)
    : QMenu(QStringLiteral("Accounts"), parent)
    , mTailscale(tailscale)
    , mRoleIndicesFound(true)
    , mRoleIndexId(tailscale->loginProfileModel()->roleIndexForProperty("id"))
    , mRoleIndexName(tailscale->loginProfileModel()->roleIndexForProperty("name"))
    , mNetworkManager(new QNetworkAccessManager(this))
{
    setIcon(QIcon::fromTheme(QStringLiteral("config-users")));
    if (mRoleIndexId == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for id";
        mRoleIndicesFound = false;
    }
    if (mRoleIndexName == -1) {
        qCCritical(Logging::TrayIcon) << "Failed to find role index for name";
        mRoleIndicesFound = false;
    }
    connect(this, &QMenu::aboutToShow, this, &TrayMenuAccounts::rebuildMenu);
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
        }
    });
}

void TrayMenuAccounts::rebuildMenu()
{
    clear();
    if (!mRoleIndicesFound) {
        return;
    }
    const QString currentId = mTailscale->currentLoginProfileId();
    const int rows = mTailscale->loginProfileModel()->rowCount(QModelIndex{});
    for (int row = 0; row < rows; ++row) {
        const QModelIndex index = mTailscale->loginProfileModel()->index(row, 0);
        const QString profileId = mTailscale->loginProfileModel()->data(index, mRoleIndexId).toString();
        const QString name = mTailscale->loginProfileModel()->data(index, mRoleIndexName).toString();

        QIcon icon = QIcon::fromTheme(QStringLiteral("user"));
        if (const LoginProfile *profile = mTailscale->loginProfileWithId(profileId)) {
            const QString picUrl = profile->userProfile()->profilePicUrl();
            if (!picUrl.isEmpty()) {
                if (mAvatarCache.contains(picUrl)) {
                    icon = mAvatarCache[picUrl];
                } else {
                    fetchAvatar(picUrl);
                }
            }
        }

        QAction *action = addAction(icon, name);
        action->setCheckable(true);
        action->setChecked(profileId == currentId);
        if (profileId != currentId) {
            connect(action, &QAction::triggered, this, [this, profileId] {
                mTailscale->switchAccount(profileId);
            });
        }
    }
}
