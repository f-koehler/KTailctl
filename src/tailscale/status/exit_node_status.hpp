#ifndef KTAILCTL_EXIT_NODE_STATUS_HPP
#define KTAILCTL_EXIT_NODE_STATUS_HPP

#include <QJsonObject>
#include <QObject>
#include <QProperty>
#include <QString>
#include <QStringList>

// https://pkg.go.dev/tailscale.com/ipn/ipnstate#ExitNodeStatus
class ExitNodeStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id BINDABLE bindableId)
    Q_PROPERTY(bool isOnline READ isOnline BINDABLE bindableIsOnline)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps BINDABLE bindableTailscaleIps)

    QProperty<QString> mId;
    QProperty<bool> mIsOnline;
    QProperty<QStringList> mTailscaleIps;

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
    [[nodiscard]] QBindable<QString> bindableId() const
    {
        return {&mId};
    }

    [[nodiscard]] QBindable<bool> bindableIsOnline() const
    {
        return {&mIsOnline};
    }
    [[nodiscard]] QBindable<QStringList> bindableTailscaleIps() const
    {
        return {&mTailscaleIps};
    }
};

#endif // KTAILCTL_EXIT_NODE_STATUS_HPP
