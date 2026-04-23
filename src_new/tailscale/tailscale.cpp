#include "tailscale.hpp"
#include "logging_tailscale.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMutexLocker>
#include <QSet>

void TailscaleNew::up() noexcept
{
    tailscale_up();
    QTimer::singleShot(200, this, [this]() {
        mStatus->refresh();
    });
}

void TailscaleNew::down() noexcept
{
    tailscale_down();
    QTimer::singleShot(200, this, [this]() {
        mStatus->refresh();
    });
}

void TailscaleNew::toggle() noexcept
{
    if ((mStatus->backendState() == Status::BackendState::Starting) || (mStatus->backendState() == Status::BackendState::Running)) {
        down();
    } else {
        up();
    }
}

LoginProfile *TailscaleNew::loginProfileWithId(const QString &id) const noexcept
{
    const auto pos = mLoginProfiles.find(id);
    if (pos == mLoginProfiles.end()) [[unlikely]] {
        return nullptr;
    }
    return pos.value();
}

void TailscaleNew::refreshLoginProfiles()
{
    QMutexLocker lock(&mMutexLoginProfiles);

    const std::unique_ptr<char, decltype(&::free)> json_str(tailscale_accounts(), &free);
    const QByteArray json_buffer(json_str.get(), ::strlen(json_str.get()));
    QJsonParseError error;
    QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
    if (error.error != QJsonParseError::NoError) {
        qCCritical(Logging::TailscaleMain) << error.errorString();
        return;
    }
    QJsonObject json_obj = json.object();

    mCurrentLoginProfileId = json_obj.value(QStringLiteral("currentID")).toString();

    if (!json_obj.contains(QStringLiteral("accounts"))) {
        // no accounts found -> clear list
        mLoginProfileModel->clear();
        mLoginProfiles.clear();
    } else {
        auto loginProfilesArray = json_obj.take(QStringLiteral("accounts")).toArray();
        QSet<QString> loginProfilesToRemove(mLoginProfiles.keyBegin(), mLoginProfiles.keyEnd());
        for (auto entry : loginProfilesArray) {
            auto obj = entry.toObject();
            const auto id = obj.value(QStringLiteral("ID")).toString();
            auto pos = mLoginProfiles.find(id);

            // create missing login profiles
            if (pos == mLoginProfiles.end()) [[unlikely]] {
                pos = mLoginProfiles.insert(id, new LoginProfile(this));
                mLoginProfileModel->addItem(pos.value());
            }

            // update login profile
            pos.value()->updateFromJson(obj);
            loginProfilesToRemove.remove(id);
        }

        // remove login profiles that are not present anymore
        for (const auto &id : loginProfilesToRemove) {
            auto pos = mLoginProfiles.find(id);
            if (pos == mLoginProfiles.end()) [[unlikely]] {
                // This should never happen
                continue;
            }
            mLoginProfileModel->removeItem(mLoginProfileModel->indexOf(pos.value()));
            pos.value()->deleteLater();
            mLoginProfiles.erase(pos);
        }
    }

    qCInfo(Logging::TailscaleMain) << "LoginProfileModel refreshed";
}
