#ifndef KTAILCTL_CLIENT_VERSION_HPP
#define KTAILCTL_CLIENT_VERSION_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>

// https://pkg.go.dev/tailscale.com@v1.92.1/tailcfg#ClientVersion
class ClientVersion : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool runningLatest READ runningLatest BINDABLE bindableRunningLatest NOTIFY runningLatestChanged)
    Q_PROPERTY(QString latestVersion READ latestVersion BINDABLE bindableLatestVersion NOTIFY latestVersionChanged)
    Q_PROPERTY(bool urgentSecurityUpdate READ urgentSecurityUpdate BINDABLE bindableUrgentSecurityUpdate NOTIFY urgentSecurityUpdateChanged)
    Q_PROPERTY(bool notify READ notify BINDABLE bindableNotify NOTIFY notifyChanged)
    Q_PROPERTY(QString notifyUrl READ notifyUrl BINDABLE bindableNotifyUrl NOTIFY notifyUrlChanged)
    Q_PROPERTY(QString notifyText READ notifyText BINDABLE bindableNotifyText NOTIFY notifyTextChanged)

public:
    explicit ClientVersion(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    explicit ClientVersion(QJsonObject &json, QObject *parent = nullptr)
        : QObject(parent)
    {
        updateFromJson(json);
    }

    void updateFromJson(QJsonObject &json);

    // Getters
    [[nodiscard]] auto runningLatest() const noexcept -> bool
    {
        return mRunningLatest;
    }

    [[nodiscard]] auto latestVersion() const noexcept -> const QString &
    {
        return mLatestVersion;
    }

    [[nodiscard]] auto urgentSecurityUpdate() const noexcept -> bool
    {
        return mUrgentSecurityUpdate;
    }

    [[nodiscard]] auto notify() const noexcept -> bool
    {
        return mNotify;
    }

    [[nodiscard]] auto notifyUrl() const noexcept -> const QString &
    {
        return mNotifyUrl;
    }

    [[nodiscard]] auto notifyText() const noexcept -> const QString &
    {
        return mNotifyText;
    }

    // Bindables
    [[nodiscard]] auto bindableRunningLatest() -> QBindable<bool>
    {
        return {&mRunningLatest};
    }

    [[nodiscard]] auto bindableLatestVersion() -> QBindable<QString>
    {
        return {&mLatestVersion};
    }

    [[nodiscard]] auto bindableUrgentSecurityUpdate() -> QBindable<bool>
    {
        return {&mUrgentSecurityUpdate};
    }

    [[nodiscard]] auto bindableNotify() -> QBindable<bool>
    {
        return {&mNotify};
    }

    [[nodiscard]] auto bindableNotifyUrl() -> QBindable<QString>
    {
        return {&mNotifyUrl};
    }

    [[nodiscard]] auto bindableNotifyText() -> QBindable<QString>
    {
        return {&mNotifyText};
    }

Q_SIGNALS:
    void runningLatestChanged();
    void latestVersionChanged();
    void urgentSecurityUpdateChanged();
    void notifyChanged();
    void notifyUrlChanged();
    void notifyTextChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(ClientVersion, bool, mRunningLatest, &ClientVersion::runningLatestChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ClientVersion, QString, mLatestVersion, &ClientVersion::latestVersionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ClientVersion, bool, mUrgentSecurityUpdate, &ClientVersion::urgentSecurityUpdateChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ClientVersion, bool, mNotify, &ClientVersion::notifyChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ClientVersion, QString, mNotifyUrl, &ClientVersion::notifyUrlChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ClientVersion, QString, mNotifyText, &ClientVersion::notifyTextChanged)
};

#endif // KTAILCTL_CLIENT_VERSION_HPP
