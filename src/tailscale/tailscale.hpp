#ifndef KTAILCTL_TAILSCALE_HPP
#define KTAILCTL_TAILSCALE_HPP

#include <QBindable>
#include <QMap>
#include <QMutex>
#include <QObject>

#include "preferences/preferences.hpp"
#include "property_list_model.hpp"
#include "status/login_profile.hpp"
#include "status/status.hpp"

class Tailscale : public QObject
{
    Q_OBJECT

public:
    using LoginProfileModel = PropertyListModel<LoginProfile, PropertyListModelOwnership::External>;

    Q_PROPERTY(Status *status READ status CONSTANT)
    Q_PROPERTY(Preferences *preferences READ preferences CONSTANT)
    Q_PROPERTY(LoginProfileModel *loginProfiles READ loginProfileModel CONSTANT)
    Q_PROPERTY(QString currentLoginProfileId READ currentLoginProfileId BINDABLE bindableCurrentLoginProfileId)

private:
    QMutex mMutexLoginProfiles;

    Status *mStatus;
    Preferences *mPreferences;
    QMap<QString, LoginProfile *> mLoginProfiles;
    LoginProfileModel *mLoginProfileModel;
    QProperty<QString> mCurrentLoginProfileId;

public Q_SLOTS:
    Q_INVOKABLE void up() const noexcept;
    Q_INVOKABLE void down() const noexcept;
    Q_INVOKABLE void toggle() const noexcept;

public:
    explicit Tailscale(QObject *parent = nullptr)
        : QObject(parent)
        , mStatus(new Status(this))
        , mPreferences(new Preferences(this))
        , mLoginProfileModel(new LoginProfileModel(this))
    {
        refreshLoginProfiles();
    }

    [[nodiscard]] Status *status() const noexcept
    {
        return mStatus;
    }

    [[nodiscard]] Preferences *preferences() const noexcept
    {
        return mPreferences;
    }

    [[nodiscard]] LoginProfileModel *loginProfileModel() const noexcept
    {
        return mLoginProfileModel;
    }

    [[nodiscard]] const QString &currentLoginProfileId() const noexcept
    {
        return mCurrentLoginProfileId;
    }

    [[nodiscard]] QBindable<QString> bindableCurrentLoginProfileId()
    {
        return {&mCurrentLoginProfileId};
    }

    Q_INVOKABLE LoginProfile *loginProfileWithId(const QString &loginProfileId) const noexcept;
    Q_INVOKABLE void refreshLoginProfiles();
};

#endif // KTAILCTL_TAILSCALE_HPP
