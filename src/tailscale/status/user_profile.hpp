#ifndef KTAILCTL_USER_PROFILE_HPP
#define KTAILCTL_USER_PROFILE_HPP

#include <QBindable>
#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

class UserProfile : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qint64 userId READ userId BINDABLE bindableUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString loginName READ loginName BINDABLE bindableLoginName NOTIFY loginNameChanged)
    Q_PROPERTY(QString displayName READ displayName BINDABLE bindableDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString profilePicUrl READ profilePicUrl BINDABLE bindableProfilePicUrl NOTIFY profilePicUrlChanged)

public:
    explicit UserProfile(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    explicit UserProfile(QJsonObject &json, QObject *parent = nullptr)
        : QObject(parent)
    {
        updateFromJson(json);
    }

    void updateFromJson(QJsonObject &json);

    // Getters
    [[nodiscard]] qint64 userId() const noexcept
    {
        return mUserId;
    }

    [[nodiscard]] const QString &loginName() const noexcept
    {
        return mLoginName;
    }

    [[nodiscard]] const QString &displayName() const noexcept
    {
        return mDisplayName;
    }

    [[nodiscard]] const QString &profilePicUrl() const noexcept
    {
        return mProfilePicUrl;
    }

    // Bindables
    [[nodiscard]] QBindable<qint64> bindableUserId()
    {
        return {&mUserId};
    }

    [[nodiscard]] QBindable<QString> bindableLoginName()
    {
        return {&mLoginName};
    }

    [[nodiscard]] QBindable<QString> bindableDisplayName()
    {
        return {&mDisplayName};
    }

    [[nodiscard]] QBindable<QString> bindableProfilePicUrl()
    {
        return {&mProfilePicUrl};
    }

Q_SIGNALS:
    void userIdChanged();
    void loginNameChanged();
    void displayNameChanged();
    void profilePicUrlChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(UserProfile, qint64, mUserId, &UserProfile::userIdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(UserProfile, QString, mLoginName, &UserProfile::loginNameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(UserProfile, QString, mDisplayName, &UserProfile::displayNameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(UserProfile, QString, mProfilePicUrl, &UserProfile::profilePicUrlChanged)
};

#endif // KTAILCTL_USER_PROFILE_HPP
