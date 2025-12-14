#ifndef KTAILCTL_STATUS_HPP
#define KTAILCTL_STATUS_HPP

#include "libktailctl_wrapper.h"

#include "client_version.hpp"
#include "exit_node_status.hpp"
#include "peer_status.hpp"
#include "tailnet_status.hpp"
#include "user_profile.hpp"

#include <QMap>
#include <QObject>
#include <QPointer>
#include <QString>

// https://pkg.go.dev/tailscale.com/ipn/ipnstate#Status
class Status : public QObject
{
    Q_OBJECT

    enum class BackendState : uint8_t { NoState, NeedsLogin, NeedsMachineAuth, Stopped, Starting, Running };

    Q_ENUM(BackendState)

    Q_PROPERTY(QString version READ version BINDABLE bindableVersion)
    Q_PROPERTY(bool isTun READ isTun BINDABLE bindableIsTun)
    Q_PROPERTY(BackendState backendState READ backendState BINDABLE bindableBackendState)
    Q_PROPERTY(bool haveNodeKey READ haveNodeKey BINDABLE bindableHaveNodeKey)
    Q_PROPERTY(QString authUrl READ authUrl BINDABLE bindableAuthUrl)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps BINDABLE bindableTailscaleIps)
    Q_PROPERTY(PeerStatus *self READ self BINDABLE bindableSelf)
    Q_PROPERTY(ExitNodeStatus *exitNodeStatus READ exitNodeStatus BINDABLE bindableExitNodeStatus)
    Q_PROPERTY(QStringList health READ health BINDABLE bindableHealth)
    Q_PROPERTY(TailnetStatus *currentTailnet READ currentTailnet BINDABLE bindableCurrentTailnet)
    Q_PROPERTY(QMap<QString, PeerStatus *> peers READ peers BINDABLE bindablePeers)
    Q_PROPERTY(QMap<qint64, UserProfile *> users READ users BINDABLE bindableUsers)
    Q_PROPERTY(ClientVersion *clientVersion READ clientVersion BINDABLE bindableClientVersion)

public:
private:
    QProperty<QString> mVersion;
    QProperty<bool> mIsTun;
    QProperty<BackendState> mBackendState;
    QProperty<bool> mHaveNodeKey;
    QProperty<QString> mAuthUrl;
    QProperty<QStringList> mTailscaleIps;
    QProperty<PeerStatus *> mSelf;
    QProperty<ExitNodeStatus *> mExitNodeStatus;
    QProperty<QStringList> mHealth;
    QProperty<TailnetStatus *> mCurrentTailnet;
    QProperty<QMap<QString, PeerStatus *>> mPeers;
    QProperty<QMap<qint64, UserProfile *>> mUsers;
    QProperty<ClientVersion *> mClientVersion;

public:
    explicit Status(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    explicit Status(QJsonObject &json, QObject *parent = nullptr)
        : QObject(parent)
    {
        updateFromJson(json);
    }

    Q_INVOKABLE void refresh()
    {
        const std::unique_ptr<char, decltype(&::free)> json_str(tailscale_status(), &free);
        const QByteArray json_buffer(json_str.get(), ::strlen(json_str.get()));
        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
        if (error.error != QJsonParseError::NoError) {
            qCritical() << error.errorString();
            return;
        }
        QJsonObject json_obj = json.object();

        updateFromJson(json_obj);
    }

    void updateFromJson(QJsonObject &json)
    {
        mVersion = json.take(QStringLiteral("Version")).toString();
        mIsTun = json.take(QStringLiteral("TUN")).toBool();
        // mBackendState =
        mHaveNodeKey = json.take(QStringLiteral("HaveNodeKey")).toBool();
        mAuthUrl = json.take(QStringLiteral("AuthUrl")).toString();
        mTailscaleIps = json.take(QStringLiteral("TailscaleIps")).toVariant().toStringList();

        if (json.contains(QStringLiteral("Self"))) {
            auto selfJson = json.take(QStringLiteral("Self")).toObject();
            if (mSelf.value() == nullptr) {
                mSelf = new PeerStatus(this);
            }
            mSelf->updateFromJson(selfJson);
        } else {
            if (mSelf.value() != nullptr) {
                mSelf->deleteLater();
                mSelf = nullptr;
            }
        }

        if (json.contains(QStringLiteral("ExitNodeStatus"))) {
            auto exitNodeStatusJson = json.take(QStringLiteral("ExitNodeStatus")).toObject();
            if (mExitNodeStatus.value() == nullptr) {
                mExitNodeStatus = new ExitNodeStatus(this);
            }
            mExitNodeStatus->updateFromJson(exitNodeStatusJson);
        } else {
            if (mExitNodeStatus.value() != nullptr) {
                mExitNodeStatus->deleteLater();
                mExitNodeStatus = nullptr;
            }
        }
    }

    // Getters
    [[nodiscard]] const QString &version() const noexcept
    {
        return mVersion;
    }

    [[nodiscard]] bool isTun() const noexcept
    {
        return mIsTun;
    }

    [[nodiscard]] BackendState backendState() const noexcept
    {
        return mBackendState;
    }

    [[nodiscard]] bool haveNodeKey() const noexcept
    {
        return mHaveNodeKey;
    }

    [[nodiscard]] const QString &authUrl() const noexcept
    {
        return mAuthUrl;
    }

    [[nodiscard]] const QStringList &tailscaleIps() const noexcept
    {
        return mTailscaleIps;
    }

    [[nodiscard]] PeerStatus *self() const noexcept
    {
        return mSelf;
    }

    [[nodiscard]] ExitNodeStatus *exitNodeStatus() const noexcept
    {
        return mExitNodeStatus;
    }

    [[nodiscard]] const QStringList &health() const noexcept
    {
        return mHealth;
    }

    [[nodiscard]] TailnetStatus *currentTailnet() const noexcept
    {
        return mCurrentTailnet;
    }

    [[nodiscard]] const QMap<QString, PeerStatus *> &peers() const noexcept
    {
        return mPeers;
    }

    [[nodiscard]] const QMap<qint64, UserProfile *> &users() const noexcept
    {
        return mUsers;
    }

    [[nodiscard]] ClientVersion *clientVersion() const noexcept
    {
        return mClientVersion;
    }

    // Bindables
    [[nodiscard]] QBindable<QString> bindableVersion()
    {
        return {&mVersion};
    }

    [[nodiscard]] QBindable<bool> bindableIsTun()
    {
        return {&mIsTun};
    }

    [[nodiscard]] QBindable<BackendState> bindableBackendState()
    {
        return {&mBackendState};
    }

    [[nodiscard]] QBindable<bool> bindableHaveNodeKey()
    {
        return {&mHaveNodeKey};
    }

    [[nodiscard]] QBindable<QString> bindableAuthUrl()
    {
        return {&mAuthUrl};
    }

    [[nodiscard]] QBindable<QStringList> bindableTailscaleIps()
    {
        return {&mTailscaleIps};
    }

    [[nodiscard]] QBindable<PeerStatus *> bindableSelf()
    {
        return {&mSelf};
    }

    [[nodiscard]] QBindable<ExitNodeStatus *> bindableExitNodeStatus()
    {
        return {&mExitNodeStatus};
    }

    [[nodiscard]] QBindable<QStringList> bindableHealth()
    {
        return {&mHealth};
    }

    [[nodiscard]] QBindable<TailnetStatus *> bindableCurrentTailnet()
    {
        return {&mCurrentTailnet};
    }

    [[nodiscard]] QBindable<QMap<QString, PeerStatus *>> bindablePeers()
    {
        return {&mPeers};
    }

    [[nodiscard]] QBindable<QMap<qint64, UserProfile *>> bindableUsers()
    {
        return {&mUsers};
    }

    [[nodiscard]] QBindable<ClientVersion *> bindableClientVersion()
    {
        return {&mClientVersion};
    }
};

#endif // KTAILCTL_STATUS_HPP
