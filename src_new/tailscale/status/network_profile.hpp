#ifndef KTAILCTL_NETWORK_PROFILE_HPP
#define KTAILCTL_NETWORK_PROFILE_HPP

#include <QObject>
#include <QProperty>

class NetworkProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString magicDnsName READ magicDnsName BINDABLE bindableMagicDnsName)
    Q_PROPERTY(QString domainName READ domainName BINDABLE bindableDomainName)
    Q_PROPERTY(QString displayName READ displayName BINDABLE bindableDisplayName)

private:
    QProperty<QString> mMagicDnsName;
    QProperty<QString> mDomainName;
    QProperty<QString> mDisplayName;

public:
    explicit NetworkProfile(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    void updateFromJson(QJsonObject &json)
    {
        mMagicDnsName = json.take(QStringLiteral("MagicDNSName")).toString();
        mDomainName = json.take(QStringLiteral("DomainName")).toString();
        mDisplayName = json.take(QStringLiteral("DisplayName")).toString();
    }

    [[nodiscard]] const QString &magicDnsName() const noexcept
    {
        return mMagicDnsName;
    }
    [[nodiscard]] const QString &domainName() const noexcept
    {
        return mDomainName;
    }
    [[nodiscard]] const QString &displayName() const noexcept
    {
        return mDisplayName;
    }

    [[nodiscard]] QBindable<QString> bindableMagicDnsName()
    {
        return {&mMagicDnsName};
    }
    [[nodiscard]] QBindable<QString> bindableDomainName()
    {
        return {&mDomainName};
    }
    [[nodiscard]] QBindable<QString> bindableDisplayName()
    {
        return {&mDisplayName};
    }
};

#endif // KTAILCTL_NETWORK_PROFILE_HPP
