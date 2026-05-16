#ifndef KTAILCTL_PEER_STATUS_HPP
#define KTAILCTL_PEER_STATUS_HPP

#include "location.hpp"
#include <QBindable>
#include <QDateTime>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QStringList>
#include <QtQmlIntegration/qqmlintegration.h>

// https://pkg.go.dev/tailscale.com/ipn/ipnstate#PeerStatus
class PeerStatus : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum class TaildropTargetStatus : uint8_t {
        Unknown,
        Available,
        NoNetmapAvailable,
        IpnStateNotRunning,
        MissingCapability,
        Offline,
        NoPeerInfo,
        UnsupportedOs,
        NoPeerApi,
        OwnedByOtherUser
    };

    Q_ENUM(TaildropTargetStatus)

    Q_PROPERTY(QString id READ id BINDABLE bindableId NOTIFY idChanged)
    Q_PROPERTY(QString publicKey READ publicKey BINDABLE bindablePublicKey NOTIFY publicKeyChanged)
    Q_PROPERTY(QString hostName READ hostName BINDABLE bindableHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString dnsName READ dnsName BINDABLE bindableDnsName NOTIFY dnsNameChanged)
    Q_PROPERTY(QString os READ os BINDABLE bindableOs NOTIFY osChanged)
    Q_PROPERTY(qint64 userId READ userId BINDABLE bindableUserId NOTIFY userIdChanged)
    Q_PROPERTY(qint64 sharerUserId READ sharerUserId BINDABLE bindableSharerUserId NOTIFY sharerUserIdChanged)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps BINDABLE bindableTailscaleIps NOTIFY tailscaleIpsChanged)
    Q_PROPERTY(QStringList allowedIps READ allowedIps BINDABLE bindableAllowedIps NOTIFY allowedIpsChanged)
    Q_PROPERTY(QStringList tags READ tags BINDABLE bindableTags NOTIFY tagsChanged)
    Q_PROPERTY(bool mullvadNode READ mullvadNode BINDABLE bindableMullvadNode NOTIFY mullvadNodeChanged)
    Q_PROPERTY(QStringList primaryRoutes READ primaryRoutes BINDABLE bindablePrimaryRoutes NOTIFY primaryRoutesChanged)
    Q_PROPERTY(QStringList addresses READ addresses BINDABLE bindableAddresses NOTIFY addressesChanged)
    Q_PROPERTY(QString currentAddress READ currentAddress BINDABLE bindableCurrentAddress NOTIFY currentAddressChanged)
    Q_PROPERTY(QString relay READ relay BINDABLE bindableRelay NOTIFY relayChanged)
    Q_PROPERTY(QString peerRelay READ peerRelay BINDABLE bindablePeerRelay NOTIFY peerRelayChanged)
    Q_PROPERTY(qint64 receivedBytes READ receivedBytes BINDABLE bindableReceivedBytes NOTIFY receivedBytesChanged)
    Q_PROPERTY(qint64 transmittedBytes READ transmittedBytes BINDABLE bindableTransmittedBytes NOTIFY transmittedBytesChanged)
    Q_PROPERTY(QDateTime created READ created BINDABLE bindableCreated NOTIFY createdChanged)
    Q_PROPERTY(QDateTime lastWrite READ lastWrite BINDABLE bindableLastWrite NOTIFY lastWriteChanged)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen BINDABLE bindableLastSeen NOTIFY lastSeenChanged)
    Q_PROPERTY(QDateTime lastHandshake READ lastHandshake BINDABLE bindableLastHandshake NOTIFY lastHandshakeChanged)
    Q_PROPERTY(bool online READ online BINDABLE bindableOnline NOTIFY onlineChanged)
    Q_PROPERTY(bool exitNode READ exitNode BINDABLE bindableExitNode NOTIFY exitNodeChanged)
    Q_PROPERTY(bool exitNodeOption READ exitNodeOption BINDABLE bindableExitNodeOption NOTIFY exitNodeOptionChanged)
    Q_PROPERTY(bool active READ active BINDABLE bindableActive NOTIFY activeChanged)
    Q_PROPERTY(QStringList peerApiUrls READ peerApiUrls BINDABLE bindablePeerApiUrls NOTIFY peerApiUrlsChanged)
    Q_PROPERTY(TaildropTargetStatus taildropTargetStatus READ taildropTargetStatus BINDABLE bindableTaildropTargetStatus NOTIFY taildropTargetStatusChanged)
    Q_PROPERTY(QString noFileSharingReason READ noFileSharingReason BINDABLE bindableNoFileSharingReason NOTIFY noFileSharingReasonChanged)
    Q_PROPERTY(QStringList sshHostKeys READ sshHostKeys BINDABLE bindableSshHostKeys NOTIFY sshHostKeysChanged)
    Q_PROPERTY(bool shareeNode READ shareeNode BINDABLE bindableShareeNode NOTIFY shareeNodeChanged)
    Q_PROPERTY(bool inNetworkMap READ inNetworkMap BINDABLE bindableInNetworkMap NOTIFY inNetworkMapChanged)
    Q_PROPERTY(bool inMagicSock READ inMagicSock BINDABLE bindableInMagicSock NOTIFY inMagicSockChanged)
    Q_PROPERTY(bool inEngine READ inEngine BINDABLE bindableInEngine NOTIFY inEngineChanged)
    Q_PROPERTY(bool expired READ expired BINDABLE bindableExpired NOTIFY expiredChanged)
    Q_PROPERTY(QDateTime keyExpiry READ keyExpiry BINDABLE bindableKeyExpiry NOTIFY keyExpiryChanged)
    Q_PROPERTY(Location *location READ location BINDABLE bindableLocation NOTIFY locationChanged)

    explicit PeerStatus(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    explicit PeerStatus(QJsonObject &json, QObject *parent = nullptr)
        : QObject(parent)
    {
        updateFromJson(json);
    }

    void updateFromJson(QJsonObject &json);

    // Getters
    [[nodiscard]] auto id() const noexcept -> const QString &
    {
        return mId;
    }

    [[nodiscard]] auto publicKey() const noexcept -> const QString &
    {
        return mPublicKey;
    }

    [[nodiscard]] auto hostName() const noexcept -> const QString &
    {
        return mHostName;
    }

    [[nodiscard]] auto dnsName() const noexcept -> const QString &
    {
        return mDnsName;
    }

    [[nodiscard]] auto os() const noexcept -> const QString &
    {
        return mOs;
    }

    [[nodiscard]] auto userId() const noexcept -> qint64
    {
        return mUserId;
    }

    [[nodiscard]] auto sharerUserId() const noexcept -> qint64
    {
        return mSharerUserId;
    }

    [[nodiscard]] auto tailscaleIps() const noexcept -> const QStringList &
    {
        return mTailscaleIps;
    }

    [[nodiscard]] auto allowedIps() const noexcept -> const QStringList &
    {
        return mAllowedIps;
    }

    [[nodiscard]] auto tags() const noexcept -> const QStringList &
    {
        return mTags;
    }

    [[nodiscard]] auto mullvadNode() const noexcept -> bool
    {
        return mMullvadNode;
    }

    [[nodiscard]] auto primaryRoutes() const noexcept -> const QStringList &
    {
        return mPrimaryRoutes;
    }

    [[nodiscard]] auto addresses() const noexcept -> const QStringList &
    {
        return mAddresses;
    }

    [[nodiscard]] auto currentAddress() const noexcept -> const QString &
    {
        return mCurrentAddress;
    }

    [[nodiscard]] auto relay() const noexcept -> const QString &
    {
        return mRelay;
    }

    [[nodiscard]] auto peerRelay() const noexcept -> const QString &
    {
        return mPeerRelay;
    }

    [[nodiscard]] auto receivedBytes() const noexcept -> qint64
    {
        return mReceivedBytes;
    }

    [[nodiscard]] auto transmittedBytes() const noexcept -> qint64
    {
        return mTransmittedBytes;
    }

    [[nodiscard]] auto created() const noexcept -> const QDateTime &
    {
        return mCreated;
    }

    [[nodiscard]] auto lastWrite() const noexcept -> const QDateTime &
    {
        return mLastWrite;
    }

    [[nodiscard]] auto lastSeen() const noexcept -> const QDateTime &
    {
        return mLastSeen;
    }

    [[nodiscard]] auto lastHandshake() const noexcept -> const QDateTime &
    {
        return mLastHandshake;
    }

    [[nodiscard]] auto online() const noexcept -> bool
    {
        return mOnline;
    }

    [[nodiscard]] auto exitNode() const noexcept -> bool
    {
        return mExitNode;
    }

    [[nodiscard]] auto exitNodeOption() const noexcept -> bool
    {
        return mExitNodeOption;
    }

    [[nodiscard]] auto active() const noexcept -> bool
    {
        return mActive;
    }

    [[nodiscard]] auto peerApiUrls() const noexcept -> const QStringList &
    {
        return mPeerApiUrls;
    }

    [[nodiscard]] auto taildropTargetStatus() const noexcept -> TaildropTargetStatus
    {
        return mTaildropTargetStatus;
    }

    [[nodiscard]] auto noFileSharingReason() const noexcept -> const QString &
    {
        return mNoFileSharingReason;
    }

    [[nodiscard]] auto sshHostKeys() const noexcept -> const QStringList &
    {
        return mSshHostKeys;
    }

    [[nodiscard]] auto shareeNode() const noexcept -> bool
    {
        return mShareeNode;
    }

    [[nodiscard]] auto inNetworkMap() const noexcept -> bool
    {
        return mInNetworkMap;
    }

    [[nodiscard]] auto inMagicSock() const noexcept -> bool
    {
        return mInMagicSock;
    }

    [[nodiscard]] auto inEngine() const noexcept -> bool
    {
        return mInEngine;
    }

    [[nodiscard]] auto expired() const noexcept -> bool
    {
        return mExpired;
    }

    [[nodiscard]] auto keyExpiry() const noexcept -> const QDateTime &
    {
        return mKeyExpiry;
    }

    [[nodiscard]] auto location() const noexcept -> Location *
    {
        return mLocation;
    }

    // Bindables
    [[nodiscard]] auto bindableId() -> QBindable<QString>
    {
        return {&mId};
    }

    [[nodiscard]] auto bindablePublicKey() -> QBindable<QString>
    {
        return {&mPublicKey};
    }

    [[nodiscard]] auto bindableHostName() -> QBindable<QString>
    {
        return {&mHostName};
    }

    [[nodiscard]] auto bindableDnsName() -> QBindable<QString>
    {
        return {&mDnsName};
    }

    [[nodiscard]] auto bindableOs() -> QBindable<QString>
    {
        return {&mOs};
    }

    [[nodiscard]] auto bindableUserId() -> QBindable<qint64>
    {
        return {&mUserId};
    }

    [[nodiscard]] auto bindableSharerUserId() -> QBindable<qint64>
    {
        return {&mSharerUserId};
    }

    [[nodiscard]] auto bindableTailscaleIps() -> QBindable<QStringList>
    {
        return {&mTailscaleIps};
    }

    [[nodiscard]] auto bindableAllowedIps() -> QBindable<QStringList>
    {
        return {&mAllowedIps};
    }

    [[nodiscard]] auto bindableTags() -> QBindable<QStringList>
    {
        return {&mTags};
    }

    [[nodiscard]] auto bindableMullvadNode() -> QBindable<bool>
    {
        return {&mMullvadNode};
    }

    [[nodiscard]] auto bindablePrimaryRoutes() -> QBindable<QStringList>
    {
        return {&mPrimaryRoutes};
    }

    [[nodiscard]] auto bindableAddresses() -> QBindable<QStringList>
    {
        return {&mAddresses};
    }

    [[nodiscard]] auto bindableCurrentAddress() -> QBindable<QString>
    {
        return {&mCurrentAddress};
    }

    [[nodiscard]] auto bindableRelay() -> QBindable<QString>
    {
        return {&mRelay};
    }

    [[nodiscard]] auto bindablePeerRelay() -> QBindable<QString>
    {
        return {&mPeerRelay};
    }

    [[nodiscard]] auto bindableReceivedBytes() -> QBindable<qint64>
    {
        return {&mReceivedBytes};
    }

    [[nodiscard]] auto bindableTransmittedBytes() -> QBindable<qint64>
    {
        return {&mTransmittedBytes};
    }

    [[nodiscard]] auto bindableCreated() -> QBindable<QDateTime>
    {
        return {&mCreated};
    }

    [[nodiscard]] auto bindableLastWrite() -> QBindable<QDateTime>
    {
        return {&mLastWrite};
    }

    [[nodiscard]] auto bindableLastSeen() -> QBindable<QDateTime>
    {
        return {&mLastSeen};
    }

    [[nodiscard]] auto bindableLastHandshake() -> QBindable<QDateTime>
    {
        return {&mLastHandshake};
    }

    [[nodiscard]] auto bindableOnline() -> QBindable<bool>
    {
        return {&mOnline};
    }

    [[nodiscard]] auto bindableExitNode() -> QBindable<bool>
    {
        return {&mExitNode};
    }

    [[nodiscard]] auto bindableExitNodeOption() -> QBindable<bool>
    {
        return {&mExitNodeOption};
    }

    [[nodiscard]] auto bindableActive() -> QBindable<bool>
    {
        return {&mActive};
    }

    [[nodiscard]] auto bindablePeerApiUrls() -> QBindable<QStringList>
    {
        return {&mPeerApiUrls};
    }

    [[nodiscard]] auto bindableTaildropTargetStatus() -> QBindable<TaildropTargetStatus>
    {
        return {&mTaildropTargetStatus};
    }

    [[nodiscard]] auto bindableNoFileSharingReason() -> QBindable<QString>
    {
        return {&mNoFileSharingReason};
    }

    [[nodiscard]] auto bindableSshHostKeys() -> QBindable<QStringList>
    {
        return {&mSshHostKeys};
    }

    [[nodiscard]] auto bindableShareeNode() -> QBindable<bool>
    {
        return {&mShareeNode};
    }

    [[nodiscard]] auto bindableInNetworkMap() -> QBindable<bool>
    {
        return {&mInNetworkMap};
    }

    [[nodiscard]] auto bindableInMagicSock() -> QBindable<bool>
    {
        return {&mInMagicSock};
    }

    [[nodiscard]] auto bindableInEngine() -> QBindable<bool>
    {
        return {&mInEngine};
    }

    [[nodiscard]] auto bindableExpired() -> QBindable<bool>
    {
        return {&mExpired};
    }

    [[nodiscard]] auto bindableKeyExpiry() -> QBindable<QDateTime>
    {
        return {&mKeyExpiry};
    }

    [[nodiscard]] auto bindableLocation() -> QBindable<Location *>
    {
        return {&mLocation};
    }

Q_SIGNALS:
    void idChanged();
    void publicKeyChanged();
    void hostNameChanged();
    void dnsNameChanged();
    void osChanged();
    void userIdChanged();
    void sharerUserIdChanged();
    void tailscaleIpsChanged();
    void allowedIpsChanged();
    void tagsChanged();
    void mullvadNodeChanged();
    void primaryRoutesChanged();
    void addressesChanged();
    void currentAddressChanged();
    void relayChanged();
    void peerRelayChanged();
    void receivedBytesChanged();
    void transmittedBytesChanged();
    void createdChanged();
    void lastWriteChanged();
    void lastSeenChanged();
    void lastHandshakeChanged();
    void onlineChanged();
    void exitNodeChanged();
    void exitNodeOptionChanged();
    void activeChanged();
    void peerApiUrlsChanged();
    void taildropTargetStatusChanged();
    void noFileSharingReasonChanged();
    void sshHostKeysChanged();
    void shareeNodeChanged();
    void inNetworkMapChanged();
    void inMagicSockChanged();
    void inEngineChanged();
    void expiredChanged();
    void keyExpiryChanged();
    void locationChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mId, &PeerStatus::idChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mPublicKey, &PeerStatus::publicKeyChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mHostName, &PeerStatus::hostNameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mDnsName, &PeerStatus::dnsNameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mOs, &PeerStatus::osChanged) // TODO(fk): replace with enum?
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, qint64, mUserId, &PeerStatus::userIdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, qint64, mSharerUserId, &PeerStatus::sharerUserIdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QStringList, mTailscaleIps, &PeerStatus::tailscaleIpsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QStringList, mAllowedIps, &PeerStatus::allowedIpsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QStringList, mTags, &PeerStatus::tagsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mMullvadNode, &PeerStatus::mullvadNodeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QStringList, mPrimaryRoutes, &PeerStatus::primaryRoutesChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QStringList, mAddresses, &PeerStatus::addressesChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mCurrentAddress, &PeerStatus::currentAddressChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mRelay, &PeerStatus::relayChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mPeerRelay, &PeerStatus::peerRelayChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, qint64, mReceivedBytes, &PeerStatus::receivedBytesChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, qint64, mTransmittedBytes, &PeerStatus::transmittedBytesChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QDateTime, mCreated, &PeerStatus::createdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QDateTime, mLastWrite, &PeerStatus::lastWriteChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QDateTime, mLastSeen, &PeerStatus::lastSeenChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QDateTime, mLastHandshake, &PeerStatus::lastHandshakeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mOnline, &PeerStatus::onlineChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mExitNode, &PeerStatus::exitNodeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mExitNodeOption, &PeerStatus::exitNodeOptionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mActive, &PeerStatus::activeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QStringList, mPeerApiUrls, &PeerStatus::peerApiUrlsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, TaildropTargetStatus, mTaildropTargetStatus, &PeerStatus::taildropTargetStatusChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QString, mNoFileSharingReason, &PeerStatus::noFileSharingReasonChanged)
    // TODO(fk): capmap
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QStringList, mSshHostKeys, &PeerStatus::sshHostKeysChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mShareeNode, &PeerStatus::shareeNodeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mInNetworkMap, &PeerStatus::inNetworkMapChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mInMagicSock, &PeerStatus::inMagicSockChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mInEngine, &PeerStatus::inEngineChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, bool, mExpired, &PeerStatus::expiredChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, QDateTime, mKeyExpiry, &PeerStatus::keyExpiryChanged)
    Q_OBJECT_BINDABLE_PROPERTY(PeerStatus, Location *, mLocation, &PeerStatus::locationChanged)
};

#endif // KTAILCTL_PEER_STATUS_HPP
