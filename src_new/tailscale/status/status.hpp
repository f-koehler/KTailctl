#ifndef KTAILCTL_STATUS_HPP
#define KTAILCTL_STATUS_HPP

#include "libktailctl_wrapper.h"
#include "logging_tailscale_status.hpp"

#include "client_version.hpp"
#include "exit_node_status.hpp"
#include "peer_status.hpp"
#include "tailnet_status.hpp"
#include "user_profile.hpp"

#include <QMap>
#include <QObject>
#include <QPointer>
#include <QString>

#include "property_list_model.hpp"

// https://pkg.go.dev/tailscale.com/ipn/ipnstate#Status
class Status : public QObject
{
    Q_OBJECT

public:
    using PeerModel = PropertyListModel<PeerStatus, PropertyListModelOwnership::External>;

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
    Q_PROPERTY(PeerModel *peers READ peerModel BINDABLE bindablePeerModel)
    Q_PROPERTY(QMap<qint64, UserProfile *> users READ users BINDABLE bindableUsers)
    Q_PROPERTY(ClientVersion *clientVersion READ clientVersion BINDABLE bindableClientVersion)

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
    QMap<QString, PeerStatus *> mPeers;
    QProperty<PeerModel *> mPeerModel;
    QProperty<QMap<qint64, UserProfile *>> mUsers;
    QProperty<ClientVersion *> mClientVersion;

public:
    explicit Status(QObject *parent = nullptr)
        : QObject(parent)
        , mPeerModel(new PeerModel(this))
    {
    }

    Q_INVOKABLE void refresh()
    {
        const std::unique_ptr<char, decltype(&::free)> json_str(tailscale_status(), &free);
        const QByteArray json_buffer(json_str.get(), ::strlen(json_str.get()));
        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
        if (error.error != QJsonParseError::NoError) {
            qCCritical(Logging::Tailscale::Status) << error.errorString();
            return;
        }
        qCInfo(Logging::Tailscale::Status) << "Status refreshed";
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

        if (json.contains(QStringLiteral("Self"))) [[likely]] {
            auto selfJson = json.take(QStringLiteral("Self")).toObject();
            if (mSelf.value() == nullptr) [[unlikely]] {
                mSelf = new PeerStatus(this);
            }
            mSelf->updateFromJson(selfJson);
        } else [[unlikely]] {
            if (mSelf.value() != nullptr) {
                mSelf->deleteLater();
                mSelf = nullptr;
            }
        }

        if (json.contains(QStringLiteral("ExitNodeStatus"))) [[likely]] {
            auto exitNodeStatusJson = json.take(QStringLiteral("ExitNodeStatus")).toObject();
            if (mExitNodeStatus.value() == nullptr) [[unlikely]] {
                mExitNodeStatus = new ExitNodeStatus(this);
            }
            mExitNodeStatus->updateFromJson(exitNodeStatusJson);
        } else [[unlikely]] {
            if (mExitNodeStatus.value() != nullptr) {
                mExitNodeStatus->deleteLater();
                mExitNodeStatus = nullptr;
            }
        }

        mHealth = json.take(QStringLiteral("Health")).toVariant().toStringList();
        if (json.contains(QStringLiteral("CurrentTailnet"))) [[likely]] {
            auto tailnetJson = json.take(QStringLiteral("CurrentTailnet")).toObject();
            if (mCurrentTailnet.value() == nullptr) [[unlikely]] {
                mCurrentTailnet = new TailnetStatus(this);
            }
            mCurrentTailnet->updateFromJson(tailnetJson);
        } else [[unlikely]] {
            if (mCurrentTailnet.value() != nullptr) {
                mCurrentTailnet->deleteLater();
                mCurrentTailnet = nullptr;
            }
        }

        if (!json.contains(QStringLiteral("Peer"))) [[unlikely]] {
            mPeerModel->clear();
            mPeers.clear();
        } else [[likely]] {
            auto peerJson = json.take(QStringLiteral("Peer")).toObject();
            QSet<QString> peersToRemove(mPeers.keyBegin(), mPeers.keyEnd());
            for (auto [id, data] : peerJson.asKeyValueRange()) {
                auto pos = mPeers.find(id.toString());
                if (pos == mPeers.end()) [[unlikely]] {
                    pos = mPeers.insert(id.toString(), new PeerStatus(this));
                    mPeerModel->addItem(pos.value());
                }
                auto obj = data.toObject();
                pos.value()->updateFromJson(obj);
                peersToRemove.remove(id.toString());
            }
            for (const auto &id : peersToRemove) {
                auto pos = mPeers.find(id);
                if (pos == mPeers.end()) [[unlikely]] {
                    // This should not happen
                    continue;
                }
                mPeerModel->removeItem(mPeerModel->indexOf(pos.value()));
                pos.value()->deleteLater();
                mPeers.erase(pos);
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

    [[nodiscard]] PeerModel *peerModel() const noexcept
    {
        return mPeerModel;
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

    [[nodiscard]] QBindable<PeerModel *> bindablePeerModel()
    {
        return {&mPeerModel};
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
