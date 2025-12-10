#ifndef KTAILCTL_CLIENT_VERSION_HPP
#define KTAILCTL_CLIENT_VERSION_HPP

#include <QBindable>
#include <QObject>
#include <QProperty>
#include <QString>
#include <QUrl>

// https://pkg.go.dev/tailscale.com@v1.92.1/tailcfg#ClientVersion
class ClientVersion : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool runningLatest READ runningLatest BINDABLE bindableRunningLatest)
    Q_PROPERTY(QString latestVersion READ latestVersion BINDABLE bindableLatestVersion)
    Q_PROPERTY(bool urgentSecurityUpdate READ urgentSecurityUpdate BINDABLE bindableUrgentSecurityUpdate)
    Q_PROPERTY(bool notify READ notify BINDABLE bindableNotify)
    Q_PROPERTY(QUrl notifyUrl READ notifyUrl BINDABLE bindableNotifyUrl)
    Q_PROPERTY(QString notifyText READ notifyText BINDABLE bindableNotifyText)

private:
    QProperty<bool> mRunningLatest;
    QProperty<QString> mLatestVersion;
    QProperty<bool> mUrgentSecurityUpdate;
    QProperty<bool> mNotify;
    QProperty<QUrl> mNotifyUrl;
    QProperty<QString> mNotifyText;

public:
    explicit ClientVersion(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

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
    [[nodiscard]] const QUrl &notifyUrl() const noexcept
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
    [[nodiscard]] QBindable<QUrl> bindableNotifyUrl()
    {
        return {&mNotifyUrl};
    }
    [[nodiscard]] QBindable<QString> bindableNotifyText()
    {
        return {&mNotifyText};
    }
};

#endif // KTAILCTL_CLIENT_VERSION_HPP
