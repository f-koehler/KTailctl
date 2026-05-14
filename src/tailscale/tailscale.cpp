#include "tailscale.hpp"
#include "logging_tailscale.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMutexLocker>
#include <QSet>

static constexpr int tailscale_toggle_refresh_delay_ms = 200;

void Tailscale::up() const noexcept
{
    tailscale_up();
    QTimer::singleShot(tailscale_toggle_refresh_delay_ms, this, [this] {
        mStatus->refresh();
    });
}

void Tailscale::down() const noexcept
{
    tailscale_down();
    QTimer::singleShot(tailscale_toggle_refresh_delay_ms, this, [this] {
        mStatus->refresh();
    });
}

void Tailscale::toggle() const noexcept
{
    if ((mStatus->backendState() == Status::BackendState::Starting) || (mStatus->backendState() == Status::BackendState::Running)) {
        down();
    } else {
        up();
    }
}

LoginProfile *Tailscale::loginProfileWithId(const QString &loginProfileId) const noexcept
{
    const auto pos = mLoginProfiles.find(loginProfileId);
    if (pos == mLoginProfiles.end()) [[unlikely]] {
        return nullptr;
    }
    return pos.value();
}

void Tailscale::refreshLoginProfiles()
{
    const QMutexLocker lock(&mMutexLoginProfiles);

    const std::unique_ptr<char, decltype(&free)> json_str(tailscale_accounts(), &free);
    if (!json_str) {
        qCWarning(Logging::TailscaleMain) << "Failed to get login profiles (access denied)";
        return;
    }
    const QByteArray json_buffer(json_str.get(), strlen(json_str.get()));
    QJsonParseError error;
    const QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
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
        QSet loginProfilesToRemove(mLoginProfiles.keyBegin(), mLoginProfiles.keyEnd());
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
