#ifndef KTAILCTL_LOGIN_PROFILE_HPP
#define KTAILCTL_LOGIN_PROFILE_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>

#include "network_profile.hpp"
#include "user_profile.hpp"

class LoginProfile : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString id READ id BINDABLE bindableId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name BINDABLE bindableName NOTIFY nameChanged)
    Q_PROPERTY(NetworkProfile *networkProfile READ networkProfile CONSTANT)
    Q_PROPERTY(QString key READ key BINDABLE bindableKey NOTIFY keyChanged)
    Q_PROPERTY(UserProfile *userProfile READ userProfile CONSTANT)
    Q_PROPERTY(QString nodeId READ nodeId BINDABLE bindableNodeId NOTIFY nodeIdChanged)
    Q_PROPERTY(QString localUserId READ localUserId BINDABLE bindableLocalUserId NOTIFY localUserIdChanged)
    Q_PROPERTY(QString controlUrl READ controlUrl BINDABLE bindableControlUrl NOTIFY controlUrlChanged)

public:
    explicit LoginProfile(QObject *parent = nullptr)
        : QObject(parent)
        , mNetworkProfile(new NetworkProfile(this))
        , mUserProfile(new UserProfile(this))
    {
    }

    void updateFromJson(QJsonObject &json);

    [[nodiscard]] auto id() const noexcept -> const QString &
    {
        return mId;
    }
    [[nodiscard]] auto name() const noexcept -> const QString &
    {
        return mName;
    }
    [[nodiscard]] auto networkProfile() const noexcept -> NetworkProfile *
    {
        return mNetworkProfile;
    }
    [[nodiscard]] auto key() const noexcept -> const QString &
    {
        return mKey;
    }
    [[nodiscard]] auto userProfile() const noexcept -> UserProfile *
    {
        return mUserProfile;
    }
    [[nodiscard]] auto nodeId() const noexcept -> const QString &
    {
        return mNodeId;
    }
    [[nodiscard]] auto localUserId() const noexcept -> const QString &
    {
        return mLocalUserId;
    }
    [[nodiscard]] auto controlUrl() const noexcept -> const QString &
    {
        return mControlUrl;
    }

    [[nodiscard]] auto bindableId() -> QBindable<QString>
    {
        return {&mId};
    }
    [[nodiscard]] auto bindableName() -> QBindable<QString>
    {
        return {&mName};
    }
    [[nodiscard]] auto bindableKey() -> QBindable<QString>
    {
        return {&mKey};
    }
    [[nodiscard]] auto bindableNodeId() -> QBindable<QString>
    {
        return {&mNodeId};
    }
    [[nodiscard]] auto bindableLocalUserId() -> QBindable<QString>
    {
        return {&mLocalUserId};
    }
    [[nodiscard]] auto bindableControlUrl() -> QBindable<QString>
    {
        return {&mControlUrl};
    }

Q_SIGNALS:
    void idChanged();
    void nameChanged();
    void keyChanged();
    void nodeIdChanged();
    void localUserIdChanged();
    void controlUrlChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(LoginProfile, QString, mId, &LoginProfile::idChanged)
    Q_OBJECT_BINDABLE_PROPERTY(LoginProfile, QString, mName, &LoginProfile::nameChanged)
    NetworkProfile *mNetworkProfile;
    Q_OBJECT_BINDABLE_PROPERTY(LoginProfile, QString, mKey, &LoginProfile::keyChanged)
    UserProfile *mUserProfile;
    Q_OBJECT_BINDABLE_PROPERTY(LoginProfile, QString, mNodeId, &LoginProfile::nodeIdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(LoginProfile, QString, mLocalUserId, &LoginProfile::localUserIdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(LoginProfile, QString, mControlUrl, &LoginProfile::controlUrlChanged)
};

#endif // KTAILCTL_LOGIN_PROFILE_HPP
