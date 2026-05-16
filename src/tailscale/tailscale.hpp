#ifndef KTAILCTL_TAILSCALE_HPP
#define KTAILCTL_TAILSCALE_HPP

#include "preferences/preferences.hpp"
#include "property_list_model.hpp"
#include "status/login_profile.hpp"
#include "status/status.hpp"
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>

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
    static Tailscale *create(QQmlEngine *, QJSEngine *)
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
