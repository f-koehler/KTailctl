#ifndef KTAILCTL_NETWORK_PROFILE_HPP
#define KTAILCTL_NETWORK_PROFILE_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QtQmlIntegration/qqmlintegration.h>

class NetworkProfile : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString magicDnsName READ magicDnsName BINDABLE bindableMagicDnsName NOTIFY magicDnsNameChanged)
    Q_PROPERTY(QString domainName READ domainName BINDABLE bindableDomainName NOTIFY domainNameChanged)
    Q_PROPERTY(QString displayName READ displayName BINDABLE bindableDisplayName NOTIFY displayNameChanged)

public:
    explicit NetworkProfile(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    void updateFromJson(QJsonObject &json);

    [[nodiscard]] auto magicDnsName() const noexcept -> const QString &
    {
        return mMagicDnsName;
    }
    [[nodiscard]] auto domainName() const noexcept -> const QString &
    {
        return mDomainName;
    }
    [[nodiscard]] auto displayName() const noexcept -> const QString &
    {
        return mDisplayName;
    }

    [[nodiscard]] auto bindableMagicDnsName() -> QBindable<QString>
    {
        return {&mMagicDnsName};
    }
    [[nodiscard]] auto bindableDomainName() -> QBindable<QString>
    {
        return {&mDomainName};
    }
    [[nodiscard]] auto bindableDisplayName() -> QBindable<QString>
    {
        return {&mDisplayName};
    }

Q_SIGNALS:
    void magicDnsNameChanged();
    void domainNameChanged();
    void displayNameChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(NetworkProfile, QString, mMagicDnsName, &NetworkProfile::magicDnsNameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(NetworkProfile, QString, mDomainName, &NetworkProfile::domainNameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(NetworkProfile, QString, mDisplayName, &NetworkProfile::displayNameChanged)
};

#endif // KTAILCTL_NETWORK_PROFILE_HPP
