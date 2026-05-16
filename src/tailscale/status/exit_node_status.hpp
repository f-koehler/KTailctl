#ifndef KTAILCTL_EXIT_NODE_STATUS_HPP
#define KTAILCTL_EXIT_NODE_STATUS_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QStringList>
#include <QtQmlIntegration/qqmlintegration.h>

// https://pkg.go.dev/tailscale.com/ipn/ipnstate#ExitNodeStatus
class ExitNodeStatus : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString id READ id BINDABLE bindableId NOTIFY idChanged)
    Q_PROPERTY(bool isOnline READ isOnline BINDABLE bindableIsOnline NOTIFY isOnlineChanged)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps BINDABLE bindableTailscaleIps NOTIFY tailscaleIpsChanged)

public:
    explicit ExitNodeStatus(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    explicit ExitNodeStatus(QJsonObject &json, QObject *parent = nullptr)
        : QObject(parent)
    {
        updateFromJson(json);
    }

    void updateFromJson(QJsonObject &json);

    // Getters
    [[nodiscard]] const QString &id() const noexcept
    {
        return mId;
    }
    [[nodiscard]] bool isOnline() const noexcept
    {
        return mIsOnline;
    }
    [[nodiscard]] const QStringList &tailscaleIps() const noexcept
    {
        return mTailscaleIps;
    }

    // Bindables
    [[nodiscard]] QBindable<QString> bindableId()
    {
        return {&mId};
    }

    [[nodiscard]] QBindable<bool> bindableIsOnline()
    {
        return {&mIsOnline};
    }
    [[nodiscard]] QBindable<QStringList> bindableTailscaleIps()
    {
        return {&mTailscaleIps};
    }

Q_SIGNALS:
    void idChanged();
    void isOnlineChanged();
    void tailscaleIpsChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(ExitNodeStatus, QString, mId, &ExitNodeStatus::idChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ExitNodeStatus, bool, mIsOnline, &ExitNodeStatus::isOnlineChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ExitNodeStatus, QStringList, mTailscaleIps, &ExitNodeStatus::tailscaleIpsChanged)
};

#endif // KTAILCTL_EXIT_NODE_STATUS_HPP
