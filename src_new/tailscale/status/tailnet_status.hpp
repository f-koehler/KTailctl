#ifndef KTAILCTL_TAILNET_STATUS_HPP
#define KTAILCTL_TAILNET_STATUS_HPP

#include <QBindable>
#include <QObject>
#include <QProperty>
#include <QString>

class TailnetStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name BINDABLE bindableName)
    Q_PROPERTY(QString magicDnsSuffix READ magicDnsSuffix BINDABLE bindableMagicDnsSuffix)
    Q_PROPERTY(bool magicDnsEnabled READ magicDnsEnabled BINDABLE bindableMagicDnsEnabled)

private:
    QProperty<QString> mName;
    QProperty<QString> mMagicDnsSuffix;
    QProperty<bool> mMagicDnsEnabled;

public:
    explicit TailnetStatus(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    // Getters
    [[nodiscard]] const QString &name() const noexcept
    {
        return mName;
    }
    [[nodiscard]] const QString &magicDnsSuffix() const noexcept
    {
        return mMagicDnsSuffix;
    }
    [[nodiscard]] bool magicDnsEnabled() const noexcept
    {
        return mMagicDnsEnabled;
    }

    // Bindables
    [[nodiscard]] QBindable<QString> bindableName()
    {
        return {&mName};
    }
    [[nodiscard]] QBindable<QString> bindableMagicDnsSuffix()
    {
        return {&mMagicDnsSuffix};
    }
    [[nodiscard]] QBindable<bool> bindableMagicDnsEnabled()
    {
        return {&mMagicDnsEnabled};
    }
};

#endif // KTAILCTL_TAILNET_STATUS_HPP