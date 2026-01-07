#ifndef KTAILCTL_LOGIN_PROFILE_HPP
#define KTAILCTL_LOGIN_PROFILE_HPP

#include "network_profile.hpp"
#include "user_profile.hpp"

#include <QObject>

class LoginProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id BINDABLE bindableId)
    Q_PROPERTY(QString name READ name BINDABLE bindableName)
    Q_PROPERTY(NetworkProfile *networkProfile READ networkProfile CONSTANT)
    Q_PROPERTY(QString key READ key BINDABLE bindableKey)
    Q_PROPERTY(UserProfile *userProfile READ userProfile CONSTANT)
    Q_PROPERTY(QString nodeId READ nodeId BINDABLE bindableNodeId)
    Q_PROPERTY(QString localUserId READ localUserId BINDABLE bindableLocalUserId)
    Q_PROPERTY(QString controlUrl READ controlUrl BINDABLE bindableControlUrl)

private:
    QProperty<QString> mId;
    QProperty<QString> mName;
    NetworkProfile *mNetworkProfile;
    QProperty<QString> mKey;
    UserProfile *mUserProfile;
    QProperty<QString> mNodeId;
    QProperty<QString> mLocalUserId;
    QProperty<QString> mControlUrl;

public:
    explicit LoginProfile(QObject *parent = nullptr)
        : QObject(parent)
        , mNetworkProfile(new NetworkProfile(this))
        , mUserProfile(new UserProfile(this))
    {
    }

    void updateFromJson(QJsonObject &json)
    {
        mId = json.take(QStringLiteral("ID")).toString();
        mName = json.take(QStringLiteral("Name")).toString();
        mKey = json.take(QStringLiteral("Key")).toString();
        mNodeId = json.take(QStringLiteral("NodeID")).toString();
        mLocalUserId = json.take(QStringLiteral("LocalUserID")).toString();
        mControlUrl = json.take(QStringLiteral("ControlURL")).toString();

        auto networkJson = json.take(QStringLiteral("NetworkProfile")).toObject();
        mNetworkProfile->updateFromJson(networkJson);
        auto userProfileJson = json.take(QStringLiteral("UserProfile")).toObject();
        mUserProfile->updateFromJson(userProfileJson);
    }

    [[nodiscard]] const QString &id() const noexcept
    {
        return mId;
    }
    [[nodiscard]] const QString &name() const noexcept
    {
        return mName;
    }
    [[nodiscard]] NetworkProfile *networkProfile() const noexcept
    {
        return mNetworkProfile;
    }
    [[nodiscard]] const QString &key() const noexcept
    {
        return mKey;
    }
    [[nodiscard]] UserProfile *userProfile() const noexcept
    {
        return mUserProfile;
    }
    [[nodiscard]] const QString &nodeId() const noexcept
    {
        return mNodeId;
    }
    [[nodiscard]] const QString &localUserId() const noexcept
    {
        return mLocalUserId;
    }
    [[nodiscard]] const QString &controlUrl() const noexcept
    {
        return mControlUrl;
    }

    [[nodiscard]] QBindable<QString> bindableId()
    {
        return {&mId};
    }
    [[nodiscard]] QBindable<QString> bindableName()
    {
        return {&mName};
    }
    [[nodiscard]] QBindable<QString> bindableKey()
    {
        return {&mKey};
    }
    [[nodiscard]] QBindable<QString> bindableNodeId()
    {
        return {&mNodeId};
    }
    [[nodiscard]] QBindable<QString> bindableLocalUserId()
    {
        return {&mLocalUserId};
    }
    [[nodiscard]] QBindable<QString> bindableControlUrl()
    {
        return {&mControlUrl};
    }
};

#endif // KTAILCTL_LOGIN_PROFILE_HPP
