#ifndef KTAILCTL_USER_PROFILE_HPP
#define KTAILCTL_USER_PROFILE_HPP

#include <QBindable>
#include <QObject>
#include <QString>

class UserProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 userId READ userId BINDABLE bindableUserId)
    Q_PROPERTY(QString loginName READ loginName BINDABLE bindableLoginName)
    Q_PROPERTY(QString displayName READ displayName BINDABLE bindableDisplayName)
    Q_PROPERTY(QString profilePicUrl READ profilePicUrl BINDABLE bindableProfilePicUrl)

private:
    QProperty<qint64> mUserId;
    QProperty<QString> mLoginName;
    QProperty<QString> mDisplayName;
    QProperty<QString> mProfilePicUrl;

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

    void updateFromJson(QJsonObject &json)
    {
        QJsonDocument doc(json);
        mUserId = json.take(QStringLiteral("ID")).toInteger();
        mLoginName = json.take(QStringLiteral("LoginName")).toString();
        mDisplayName = json.take(QStringLiteral("DisplayName")).toString();
        mProfilePicUrl = json.take(QStringLiteral("ProfilePicURL")).toString();
    }

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
};

#endif // KTAILCTL_USER_PROFILE_HPP
