#include "tailscale.hpp"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QLoggingCategory>
#include <QMessageLogger>
#include <QMutexLocker>
#include <QSet>
#include <QStringLiteral>
#include <QTimer>
#include <cstdlib>
#include <cstring>
#include <libktailctl_wrapper.h>
#include <memory>
#include <qtypes.h>
#include <utility>

#include "logging_tailscale.hpp"
#include "login_profile.hpp"
#include "status.hpp"

static constexpr int tailscale_toggle_refresh_delay_ms = 200;

void Tailscale::up() const noexcept
{
    tailscale_up();
    QTimer::singleShot(tailscale_toggle_refresh_delay_ms, this, [this] -> void {
        mStatus->refresh();
    });
}

void Tailscale::down() const noexcept
{
    tailscale_down();
    QTimer::singleShot(tailscale_toggle_refresh_delay_ms, this, [this] -> void {
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

auto Tailscale::loginProfileWithId(const QString &loginProfileId) const noexcept -> LoginProfile *
{
    const auto pos = mLoginProfiles.find(loginProfileId);
    if (pos == mLoginProfiles.end()) [[unlikely]] {
        return nullptr;
    }
    return pos.value();
}

auto Tailscale::pinger(const QString &address) -> Pinger *
{
    auto pos = mPingers.find(address);
    if (pos == mPingers.end()) {
        pos = mPingers.insert(address, new Pinger(address, this));
    }
    return pos.value();
}

void Tailscale::switchAccount(const QString &profileId) noexcept
{
    const QByteArray profileIdUtf8 = profileId.toUtf8();
    GoString goString;
    goString.p = profileIdUtf8.constData();
    goString.n = static_cast<GoInt>(profileIdUtf8.size());
    if (tailscale_switch_account(&goString) == 0U) {
        qCCritical(Logging::TailscaleMain) << "Failed to switch account to" << profileId;
        return;
    }
    refreshLoginProfiles();
    mStatus->refresh();
}

void Tailscale::refreshLoginProfiles()
{
    const QMutexLocker lock(&mMutexLoginProfiles);

    const std::unique_ptr<char, decltype(&free)> json_str(tailscale_accounts(), &free);
    if (!json_str) {
        qCWarning(Logging::TailscaleMain) << "Failed to get login profiles (access denied)";
        return;
    }
    const QByteArray json_buffer(json_str.get(), static_cast<qsizetype>(strlen(json_str.get())));
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
        for (const auto entry : std::as_const(loginProfilesArray)) {
            auto obj = entry.toObject();
            const auto profileId = obj.value(QStringLiteral("ID")).toString();
            auto pos = mLoginProfiles.find(profileId);

            // create missing login profiles
            if (pos == mLoginProfiles.end()) [[unlikely]] {
                pos = mLoginProfiles.insert(profileId, new LoginProfile(this));
                mLoginProfileModel->addItem(pos.value());
            }

            // update login profile
            pos.value()->updateFromJson(obj);
            loginProfilesToRemove.remove(profileId);
        }

        // remove login profiles that are not present anymore
        for (const auto &profileId : loginProfilesToRemove) {
            auto pos = mLoginProfiles.find(profileId);
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
