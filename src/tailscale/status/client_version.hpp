#ifndef KTAILCTL_CLIENT_VERSION_HPP
#define KTAILCTL_CLIENT_VERSION_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

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
    [[nodiscard]] bool runningLatest() const noexcept
    {
        return mRunningLatest;
    }

    [[nodiscard]] const QString &latestVersion() const noexcept
    {
        return mLatestVersion;
    }

    [[nodiscard]] bool urgentSecurityUpdate() const noexcept
    {
        return mUrgentSecurityUpdate;
    }

    [[nodiscard]] bool notify() const noexcept
    {
        return mNotify;
    }

    [[nodiscard]] const QString &notifyUrl() const noexcept
    {
        return mNotifyUrl;
    }

    [[nodiscard]] const QString &notifyText() const noexcept
    {
        return mNotifyText;
    }

    // Bindables
    [[nodiscard]] QBindable<bool> bindableRunningLatest()
    {
        return {&mRunningLatest};
    }

    [[nodiscard]] QBindable<QString> bindableLatestVersion()
    {
        return {&mLatestVersion};
    }

    [[nodiscard]] QBindable<bool> bindableUrgentSecurityUpdate()
    {
        return {&mUrgentSecurityUpdate};
    }

    [[nodiscard]] QBindable<bool> bindableNotify()
    {
        return {&mNotify};
    }

    [[nodiscard]] QBindable<QString> bindableNotifyUrl()
    {
        return {&mNotifyUrl};
    }

    [[nodiscard]] QBindable<QString> bindableNotifyText()
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
