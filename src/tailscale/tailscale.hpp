#ifndef KTAILCTL_TAILSCALE_HPP
#define KTAILCTL_TAILSCALE_HPP

#include <QJSEngine>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qproperty.h>
#include <qtmetamacros.h>
#include <qttypetraits.h>

#include "preferences/preferences.hpp"
#include "property_list_model.hpp"
#include "status/login_profile.hpp"
#include "status/status.hpp"

using LoginProfileModelBase = PropertyListModel<LoginProfile, PropertyListModelOwnership::External>;

class LoginProfileModel : public LoginProfileModelBase
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using LoginProfileModelBase::LoginProfileModelBase;
};

class Tailscale : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    using LoginProfileModel = ::LoginProfileModel;

    inline static Tailscale *s_instance = nullptr;
    static auto create(QQmlEngine * /*engine*/, QJSEngine * /*jsEngine*/) -> Tailscale *
    {
        return s_instance;
    }
    static void setQmlInstance(Tailscale *instance)
    {
        s_instance = instance;
    }

    Q_PROPERTY(Status *status READ status CONSTANT)
    Q_PROPERTY(Preferences *preferences READ preferences CONSTANT)
    Q_PROPERTY(LoginProfileModel *loginProfiles READ loginProfileModel CONSTANT)
    Q_PROPERTY(QString currentLoginProfileId READ currentLoginProfileId BINDABLE bindableCurrentLoginProfileId NOTIFY currentLoginProfileIdChanged)

Q_SIGNALS:
    void currentLoginProfileIdChanged();

private:
    QMutex mMutexLoginProfiles;

    Status *mStatus;
    Preferences *mPreferences;
    QMap<QString, LoginProfile *> mLoginProfiles;
    LoginProfileModel *mLoginProfileModel;
    Q_OBJECT_BINDABLE_PROPERTY(Tailscale, QString, mCurrentLoginProfileId, &Tailscale::currentLoginProfileIdChanged)

public:
    explicit Tailscale(QObject *parent = nullptr)
        : QObject(parent)
        , mStatus(new Status(this))
        , mPreferences(new Preferences(this))
        , mLoginProfileModel(new LoginProfileModel(this))
    {
        refreshLoginProfiles();
    }

    [[nodiscard]] auto status() const noexcept -> Status *
    {
        return mStatus;
    }

    [[nodiscard]] auto preferences() const noexcept -> Preferences *
    {
        return mPreferences;
    }

    [[nodiscard]] auto loginProfileModel() const noexcept -> LoginProfileModel *
    {
        return mLoginProfileModel;
    }

    [[nodiscard]] auto currentLoginProfileId() const noexcept -> const QString &
    {
        return mCurrentLoginProfileId;
    }

    [[nodiscard]] auto bindableCurrentLoginProfileId() -> QBindable<QString>
    {
        return {&mCurrentLoginProfileId};
    }

    Q_INVOKABLE LoginProfile *loginProfileWithId(const QString &loginProfileId) const noexcept; // NOLINT(modernize-use-trailing-return-type)
    Q_INVOKABLE void refreshLoginProfiles();

    Q_SLOT Q_INVOKABLE void up() const noexcept;
    Q_SLOT Q_INVOKABLE void down() const noexcept;
    Q_SLOT Q_INVOKABLE void toggle() const noexcept;
    Q_INVOKABLE void switchAccount(const QString &profileId) noexcept;
};

#endif // KTAILCTL_TAILSCALE_HPP
