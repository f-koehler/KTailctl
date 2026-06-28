#include "status.hpp"

#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QKeyValueIterator>
#include <QLoggingCategory>
#include <QMessageLogger>
#include <QMutexLocker>
#include <QSet>
#include <QStringLiteral>
#include <QVariant>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <utility>

#include "exit_node_status.hpp"
#include "libktailctl_wrapper.h"
#include "logging_tailscale_status.hpp"
#include "mullvad_exit_node_model.hpp"
#include "peer_status.hpp"
#include "self_hosted_exit_node_model.hpp"
#include "tailnet_status.hpp"
#include <KNotification>

Status::Status(QObject *parent)
    : QObject(parent)
    , mPeerModel(new PeerModel(this))
    , mMullvadExitNodeModel(new MullvadExitNodeModel(mPeerModel, this))
    , mSelfHostedExitNodeModel(new SelfHostedExitNodeModel(mPeerModel, this))
{
    refresh();

    connect(this, &Status::backendStateChanged, this, [this] {
        if(!Config::enableNotifications()) {return;}
        if(!Config::enableTailscaleStatusNotifications()) {return;}
        if (mBackendState == Status::BackendState::Running) {
            KNotification *notification = new KNotification(QStringLiteral("tailscaleOnline"));
            notification->setText(QStringLiteral("Tailscale is online"));
            notification->sendEvent();
            return;
        }
        KNotification *notification = new KNotification(QStringLiteral("tailscaleOffline"));
        notification->setText(QStringLiteral("Tailscale is offline"));
        notification->sendEvent();
    });
    connect(this, &Status::exitNodeStatusChanged, this, [this] {
        if(!Config::enableNotifications()) {return;}
        if(!Config::enableExitNodeNotifications()) {return;}
        if (mExitNodeStatus == nullptr) {
            KNotification *notification = new KNotification(QStringLiteral("exitNodeDisabled"));
            notification->setText(QStringLiteral("Exit node disabled"));
            notification->sendEvent();
        } else {
            KNotification *notification = new KNotification(QStringLiteral("exitNodeEnabled"));
            notification->setText(QStringLiteral("Exit node enabled"));
            notification->sendEvent();
        }
    });
}

auto Status::peerWithId(const QString &peerId) const noexcept -> PeerStatus *
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

void Status::refresh() // NOLINT(readability-function-cognitive-complexity)
{
    bool emitBackendStateChanged = false;
    bool emitExitNodeStatusChanged = false;

    {
        const QMutexLocker lock(&mMutex);

        if (tailscale_daemon_running() == 0) {
            emitBackendStateChanged = mDaemonRunning; // daemon just stopped
            mDaemonRunning = false;
            qCWarning(Logging::Tailscale::Status) << "Failed to get tailscale status (daemon not running)";
        } else {
            mDaemonRunning = true;

            const std::unique_ptr<char, decltype(&free)> json_str(tailscale_status(), &free);
            if (!json_str) {
                qCWarning(Logging::Tailscale::Status) << "Failed to get tailscale status (access denied)";
            } else {
                const QByteArray json_buffer(json_str.get(), static_cast<qsizetype>(strlen(json_str.get())));
                QJsonParseError error;
                const QJsonDocument json = QJsonDocument::fromJson(json_buffer, &error);
                if (error.error != QJsonParseError::NoError) {
                    qCCritical(Logging::Tailscale::Status) << error.errorString();
                } else {
                    QJsonObject json_obj = json.object();

                    auto *const oldExitNodeStatus = mExitNodeStatus.value();

                    updateFromJson(json_obj);

                    emitExitNodeStatusChanged = (mExitNodeStatus.value() != oldExitNodeStatus);

                    const std::unique_ptr<char, decltype(&free)> suggested(tailscale_suggest_exit_node(), &free);
                    if (suggested && *suggested != '\0') {
                        mSuggestedExitNodeId = QString::fromUtf8(suggested.get());
                    } else {
                        mSuggestedExitNodeId = QString();
                    }

                    qCInfo(Logging::Tailscale::Status) << "Status refreshed";
                }
            }
        }
    }

    if (emitBackendStateChanged) {
        qCDebug(Logging::Tailscale::Status) << "Emitting backendStateChanged";
        Q_EMIT backendStateChanged();
    }
    if (emitExitNodeStatusChanged) {
        qCDebug(Logging::Tailscale::Status) << "Emitting exitNodeStatusChanged";
        Q_EMIT exitNodeStatusChanged();
    }
}

void Status::updateFromJson(QJsonObject &json) // NOLINT(readability-function-cognitive-complexity)
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
        qCDebug(Logging::Tailscale::Status) << "BackendState:" << old << "->" << mBackendState.value();
        if (mBackendState.value() != old) {
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
        QSet peersToRemove(mPeers.keyBegin(), mPeers.keyEnd());
        for (auto [_id, data] : peerJson.asKeyValueRange()) {
            auto obj = data.toObject();
            const auto peerId = obj.value(QStringLiteral("ID")).toString();
            auto pos = mPeers.find(peerId);
            const bool isNew = pos == mPeers.end();
            if (isNew) [[unlikely]] {
                pos = mPeers.insert(peerId, new PeerStatus(this));
            }
            pos.value()->updateFromJson(obj);
            if (isNew) [[unlikely]] {
                mPeerModel->addItem(pos.value());
            }
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
