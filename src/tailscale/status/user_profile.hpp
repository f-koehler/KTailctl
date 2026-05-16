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
    [[nodiscard]] auto userId() const noexcept -> qint64
    {
        return mUserId;
    }

    [[nodiscard]] auto loginName() const noexcept -> const QString &
    {
        return mLoginName;
    }

    [[nodiscard]] auto displayName() const noexcept -> const QString &
    {
        return mDisplayName;
    }

    [[nodiscard]] auto profilePicUrl() const noexcept -> const QString &
    {
        return mProfilePicUrl;
    }

    // Bindables
    [[nodiscard]] auto bindableUserId() -> QBindable<qint64>
    {
        return {&mUserId};
    }

    [[nodiscard]] auto bindableLoginName() -> QBindable<QString>
    {
        return {&mLoginName};
    }

    [[nodiscard]] auto bindableDisplayName() -> QBindable<QString>
    {
        return {&mDisplayName};
    }

    [[nodiscard]] auto bindableProfilePicUrl() -> QBindable<QString>
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
