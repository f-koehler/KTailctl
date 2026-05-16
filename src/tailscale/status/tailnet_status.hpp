#ifndef KTAILCTL_TAILNET_STATUS_HPP
#define KTAILCTL_TAILNET_STATUS_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

class TailnetStatus : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name BINDABLE bindableName NOTIFY nameChanged)
    Q_PROPERTY(QString magicDnsSuffix READ magicDnsSuffix BINDABLE bindableMagicDnsSuffix NOTIFY magicDnsSuffixChanged)
    Q_PROPERTY(bool magicDnsEnabled READ magicDnsEnabled BINDABLE bindableMagicDnsEnabled NOTIFY magicDnsEnabledChanged)

public:
    explicit TailnetStatus(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    explicit TailnetStatus(QJsonObject &json, QObject *parent = nullptr)
        : QObject(parent)
    {
        updateFromJson(json);
    }

    void updateFromJson(QJsonObject &json);

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

Q_SIGNALS:
    void nameChanged();
    void magicDnsSuffixChanged();
    void magicDnsEnabledChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(TailnetStatus, QString, mName, &TailnetStatus::nameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(TailnetStatus, QString, mMagicDnsSuffix, &TailnetStatus::magicDnsSuffixChanged)
    Q_OBJECT_BINDABLE_PROPERTY(TailnetStatus, bool, mMagicDnsEnabled, &TailnetStatus::magicDnsEnabledChanged)
};

#endif // KTAILCTL_TAILNET_STATUS_HPP
