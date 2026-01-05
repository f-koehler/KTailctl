#ifndef KTAILCTL_TAILSCALE_NEW_HPP
#define KTAILCTL_TAILSCALE_NEW_HPP

#include <QObject>
#include <QJsonDocument>
#include <QBindable>

#include "property_list_model.hpp"
#include "status/status.hpp"
#include "logging_tailscale.hpp"

class TailscaleNew : public QObject
{
    Q_OBJECT

public:
    using LoginProfileModel = PropertyListModel<LoginProfile, PropertyListModelOwnership::External>;

    Q_PROPERTY(Status *status READ status CONSTANT)
    Q_PROPERTY(LoginProfileModel *loginProfiles READ loginProfileModel CONSTANT)
    Q_PROPERTY(QString currentLoginProfileId READ currentLoginProfileId BINDABLE bindableCurrentLoginProfileId)

private:
    Status *mStatus;
    QMap<QString, LoginProfile *> mLoginProfiles;
    LoginProfileModel * mLoginProfileModel;
    QProperty<QString> mCurrentLoginProfileId;

public:
    explicit TailscaleNew(QObject *parent = nullptr)
        : QObject(parent)
        , mStatus(new Status(this))
        , mLoginProfileModel(new LoginProfileModel(this))
    {
        refreshLoginProfiles();
    }

    [[nodiscard]] Status *status() const noexcept
    {
        return mStatus;
    }

    [[nodiscard]] LoginProfileModel *loginProfileModel() const noexcept
    {
        return mLoginProfileModel;
    }

    [[nodiscard]] const QString& currentLoginProfileId() const noexcept
    {
        return mCurrentLoginProfileId;
    }

    [[nodiscard]] QBindable<QString> bindableCurrentLoginProfileId()
    {
        return {&mCurrentLoginProfileId};
    }

    Q_INVOKABLE void refreshLoginProfiles()
    {
        const std::unique_ptr<char, decltype(&::free)> json_str(tailscale_accounts(), &free);
        const QByteArray json_buffer(json_str.get(), ::strlen(json_str.get()));
        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
        if (error.error != QJsonParseError::NoError) {
            qCCritical(Logging::TailscaleMain) << error.errorString();
            return;
        }
        QJsonObject json_obj = json.object();

        if (!json_obj.contains(QStringLiteral("accounts")))  {
            // no accounts found -> clear list
            mLoginProfileModel->clear();
            mLoginProfiles.clear();
        } else  {
            auto loginProfilesArray = json_obj.take(QStringLiteral("accounts")).toArray();
            QSet<QString> loginProfilesToRemove(mLoginProfiles.keyBegin(), mLoginProfiles.keyEnd());
            for (auto entry : loginProfilesArray) {
                auto obj = entry.toObject();
                const auto id = obj.value(QStringLiteral("ID")).toString();
                auto pos = mLoginProfiles.find(id);

                // create missing login profiles
                if (pos == mLoginProfiles.end())  [[unlikely]]{
                    pos = mLoginProfiles.insert(id, new LoginProfile(this));
                    mLoginProfileModel->addItem(pos.value());
                }

                // update login profile
                pos.value()->updateFromJson(obj);
                loginProfilesToRemove.remove(id);
            }

            // remove login profiles that are not present anymore
            for (const auto& id : loginProfilesToRemove) {
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
};

#endif // KTAILCTL_TAILSCALE_HPP
