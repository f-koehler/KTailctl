#ifndef KTAILCTL_TAILNET_STATUS_HPP
#define KTAILCTL_TAILNET_STATUS_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>

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
    [[nodiscard]] auto name() const noexcept -> const QString &
    {
        return mName;
    }

    [[nodiscard]] auto magicDnsSuffix() const noexcept -> const QString &
    {
        return mMagicDnsSuffix;
    }

    [[nodiscard]] auto magicDnsEnabled() const noexcept -> bool
    {
        return mMagicDnsEnabled;
    }

    // Bindables
    [[nodiscard]] auto bindableName() -> QBindable<QString>
    {
        return {&mName};
    }

    [[nodiscard]] auto bindableMagicDnsSuffix() -> QBindable<QString>
    {
        return {&mMagicDnsSuffix};
    }

    [[nodiscard]] auto bindableMagicDnsEnabled() -> QBindable<bool>
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
