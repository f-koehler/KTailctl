#include "status.hpp"
#include "libktailctl_wrapper.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMutexLocker>
#include <QSet>

Status::Status(QObject *parent)
    : QObject(parent)
    , mPeerModel(new PeerModel(this))
    , mMullvadExitNodeModel(new MullvadExitNodeModel(mPeerModel, this))
    , mSelfHostedExitNodeModel(new SelfHostedExitNodeModel(mPeerModel, this))
{
    refresh();
}

PeerStatus *Status::peerWithId(const QString &peerId) const noexcept
{
    if (mSelf != nullptr) [[likely]] {
        if (mSelf->id() == peerId) {
            return mSelf;
        }
    }
    const auto pos = mPeers.find(peerId);
    if (pos == mPeers.end()) [[unlikely]] {
        return nullptr;
    }
    return pos.value();
}

void Status::refresh()
{
    const QMutexLocker lock(&mMutex);

    if (!tailscale_daemon_running()) {
        mDaemonRunning = false;
        qCWarning(Logging::Tailscale::Status) << "Failed to get tailscale status (daemon not running)";
        return;
    }
    mDaemonRunning = true;

    const std::unique_ptr<char, decltype(&free)> json_str(tailscale_status(), &free);
    if (!json_str) {
        qCWarning(Logging::Tailscale::Status) << "Failed to get tailscale status (access denied)";
        return;
    }
    const QByteArray json_buffer(json_str.get(), strlen(json_str.get()));
    QJsonParseError error;
    const QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
    if (error.error != QJsonParseError::NoError) {
        qCCritical(Logging::Tailscale::Status) << error.errorString();
        return;
    }
    QJsonObject json_obj = json.object();

    updateFromJson(json_obj);

    const std::unique_ptr<char, decltype(&free)> suggested(tailscale_suggest_exit_node(), &free);
    if (suggested && suggested.get()[0] != '\0') {
        mSuggestedExitNodeId = QString::fromUtf8(suggested.get());
    } else {
        mSuggestedExitNodeId = QString();
    }

    qCInfo(Logging::Tailscale::Status) << "Status refreshed";
}

void Status::updateFromJson(QJsonObject &json)
{
    mVersion = json.take(QStringLiteral("Version")).toString();
    mIsTun = json.take(QStringLiteral("TUN")).toBool();
    {
        const auto str = json.take(QStringLiteral("BackendState")).toString();
        const auto old = mBackendState.value();
        if (str == QStringLiteral("NoState")) {
            mBackendState = BackendState::NoState;
        } else if (str == QStringLiteral("NeedsLogin")) {
            mBackendState = BackendState::NeedsLogin;
        } else if (str == QStringLiteral("NeedsMachineAuth")) {
            mBackendState = BackendState::NeedsMachineAuth;
        } else if (str == QStringLiteral("Stopped")) {
            mBackendState = BackendState::Stopped;
        } else if (str == QStringLiteral("Starting")) {
            mBackendState = BackendState::Starting;
        } else if (str == QStringLiteral("Running")) {
            mBackendState = BackendState::Running;
        } else {
            qCCritical(Logging::Tailscale::Status) << "Unknown BackendState value:" << str;
            mBackendState = BackendState::NoState;
        }
        if (mBackendState != old) {
            Q_EMIT backendStateChanged();
        }
    }
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
            Q_EMIT exitNodeStatusChanged();
        }
        mExitNodeStatus->updateFromJson(exitNodeStatusJson);
    } else [[unlikely]] {
        if (mExitNodeStatus.value() != nullptr) {
            mExitNodeStatus->deleteLater();
            mExitNodeStatus = nullptr;
            Q_EMIT exitNodeStatusChanged();
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
        QSet peersToRemove(mPeers.keyBegin(), mPeers.keyEnd());
        for (auto [_id, data] : peerJson.asKeyValueRange()) {
            auto obj = data.toObject();
            const auto peerId = obj.value(QStringLiteral("ID")).toString();
            auto pos = mPeers.find(peerId);
            if (pos == mPeers.end()) [[unlikely]] {
                pos = mPeers.insert(peerId, new PeerStatus(this));
                mPeerModel->addItem(pos.value());
            }
            pos.value()->updateFromJson(obj);
            peersToRemove.remove(peerId);
        }
        for (const auto &peerId : peersToRemove) {
            auto pos = mPeers.find(peerId);
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
