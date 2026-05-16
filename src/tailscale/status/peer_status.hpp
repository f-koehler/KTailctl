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

public:
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
    [[nodiscard]] const QString &id() const noexcept
    {
        return mId;
    }

    [[nodiscard]] const QString &publicKey() const noexcept
    {
        return mPublicKey;
    }

    [[nodiscard]] const QString &hostName() const noexcept
    {
        return mHostName;
    }

    [[nodiscard]] const QString &dnsName() const noexcept
    {
        return mDnsName;
    }

    [[nodiscard]] const QString &os() const noexcept
    {
        return mOs;
    }

    [[nodiscard]] qint64 userId() const noexcept
    {
        return mUserId;
    }

    [[nodiscard]] qint64 sharerUserId() const noexcept
    {
        return mSharerUserId;
    }

    [[nodiscard]] const QStringList &tailscaleIps() const noexcept
    {
        return mTailscaleIps;
    }

    [[nodiscard]] const QStringList &allowedIps() const noexcept
    {
        return mAllowedIps;
    }

    [[nodiscard]] const QStringList &tags() const noexcept
    {
        return mTags;
    }

    [[nodiscard]] bool mullvadNode() const noexcept
    {
        return mMullvadNode;
    }

    [[nodiscard]] const QStringList &primaryRoutes() const noexcept
    {
        return mPrimaryRoutes;
    }

    [[nodiscard]] const QStringList &addresses() const noexcept
    {
        return mAddresses;
    }

    [[nodiscard]] const QString &currentAddress() const noexcept
    {
        return mCurrentAddress;
    }

    [[nodiscard]] const QString &relay() const noexcept
    {
        return mRelay;
    }

    [[nodiscard]] const QString &peerRelay() const noexcept
    {
        return mPeerRelay;
    }

    [[nodiscard]] qint64 receivedBytes() const noexcept
    {
        return mReceivedBytes;
    }

    [[nodiscard]] qint64 transmittedBytes() const noexcept
    {
        return mTransmittedBytes;
    }

    [[nodiscard]] const QDateTime &created() const noexcept
    {
        return mCreated;
    }

    [[nodiscard]] const QDateTime &lastWrite() const noexcept
    {
        return mLastWrite;
    }

    [[nodiscard]] const QDateTime &lastSeen() const noexcept
    {
        return mLastSeen;
    }

    [[nodiscard]] const QDateTime &lastHandshake() const noexcept
    {
        return mLastHandshake;
    }

    [[nodiscard]] bool online() const noexcept
    {
        return mOnline;
    }

    [[nodiscard]] bool exitNode() const noexcept
    {
        return mExitNode;
    }

    [[nodiscard]] bool exitNodeOption() const noexcept
    {
        return mExitNodeOption;
    }

    [[nodiscard]] bool active() const noexcept
    {
        return mActive;
    }

    [[nodiscard]] const QStringList &peerApiUrls() const noexcept
    {
        return mPeerApiUrls;
    }

    [[nodiscard]] TaildropTargetStatus taildropTargetStatus() const noexcept
    {
        return mTaildropTargetStatus;
    }

    [[nodiscard]] const QString &noFileSharingReason() const noexcept
    {
        return mNoFileSharingReason;
    }

    [[nodiscard]] const QStringList &sshHostKeys() const noexcept
    {
        return mSshHostKeys;
    }

    [[nodiscard]] bool shareeNode() const noexcept
    {
        return mShareeNode;
    }

    [[nodiscard]] bool inNetworkMap() const noexcept
    {
        return mInNetworkMap;
    }

    [[nodiscard]] bool inMagicSock() const noexcept
    {
        return mInMagicSock;
    }

    [[nodiscard]] bool inEngine() const noexcept
    {
        return mInEngine;
    }

    [[nodiscard]] bool expired() const noexcept
    {
        return mExpired;
    }

    [[nodiscard]] const QDateTime &keyExpiry() const noexcept
    {
        return mKeyExpiry;
    }

    [[nodiscard]] Location *location() const noexcept
    {
        return mLocation;
    }

    // Bindables
    [[nodiscard]] QBindable<QString> bindableId()
    {
        return {&mId};
    }

    [[nodiscard]] QBindable<QString> bindablePublicKey()
    {
        return {&mPublicKey};
    }

    [[nodiscard]] QBindable<QString> bindableHostName()
    {
        return {&mHostName};
    }

    [[nodiscard]] QBindable<QString> bindableDnsName()
    {
        return {&mDnsName};
    }

    [[nodiscard]] QBindable<QString> bindableOs()
    {
        return {&mOs};
    }

    [[nodiscard]] QBindable<qint64> bindableUserId()
    {
        return {&mUserId};
    }

    [[nodiscard]] QBindable<qint64> bindableSharerUserId()
    {
        return {&mSharerUserId};
    }

    [[nodiscard]] QBindable<QStringList> bindableTailscaleIps()
    {
        return {&mTailscaleIps};
    }

    [[nodiscard]] QBindable<QStringList> bindableAllowedIps()
    {
        return {&mAllowedIps};
    }

    [[nodiscard]] QBindable<QStringList> bindableTags()
    {
        return {&mTags};
    }

    [[nodiscard]] QBindable<bool> bindableMullvadNode()
    {
        return {&mMullvadNode};
    }

    [[nodiscard]] QBindable<QStringList> bindablePrimaryRoutes()
    {
        return {&mPrimaryRoutes};
    }

    [[nodiscard]] QBindable<QStringList> bindableAddresses()
    {
        return {&mAddresses};
    }

    [[nodiscard]] QBindable<QString> bindableCurrentAddress()
    {
        return {&mCurrentAddress};
    }

    [[nodiscard]] QBindable<QString> bindableRelay()
    {
        return {&mRelay};
    }

    [[nodiscard]] QBindable<QString> bindablePeerRelay()
    {
        return {&mPeerRelay};
    }

    [[nodiscard]] QBindable<qint64> bindableReceivedBytes()
    {
        return {&mReceivedBytes};
    }

    [[nodiscard]] QBindable<qint64> bindableTransmittedBytes()
    {
        return {&mTransmittedBytes};
    }

    [[nodiscard]] QBindable<QDateTime> bindableCreated()
    {
        return {&mCreated};
    }

    [[nodiscard]] QBindable<QDateTime> bindableLastWrite()
    {
        return {&mLastWrite};
    }

    [[nodiscard]] QBindable<QDateTime> bindableLastSeen()
    {
        return {&mLastSeen};
    }

    [[nodiscard]] QBindable<QDateTime> bindableLastHandshake()
    {
        return {&mLastHandshake};
    }

    [[nodiscard]] QBindable<bool> bindableOnline()
    {
        return {&mOnline};
    }

    [[nodiscard]] QBindable<bool> bindableExitNode()
    {
        return {&mExitNode};
    }

    [[nodiscard]] QBindable<bool> bindableExitNodeOption()
    {
        return {&mExitNodeOption};
    }

    [[nodiscard]] QBindable<bool> bindableActive()
    {
        return {&mActive};
    }

    [[nodiscard]] QBindable<QStringList> bindablePeerApiUrls()
    {
        return {&mPeerApiUrls};
    }

    [[nodiscard]] QBindable<TaildropTargetStatus> bindableTaildropTargetStatus()
    {
        return {&mTaildropTargetStatus};
    }

    [[nodiscard]] QBindable<QString> bindableNoFileSharingReason()
    {
        return {&mNoFileSharingReason};
    }

    [[nodiscard]] QBindable<QStringList> bindableSshHostKeys()
    {
        return {&mSshHostKeys};
    }

    [[nodiscard]] QBindable<bool> bindableShareeNode()
    {
        return {&mShareeNode};
    }

    [[nodiscard]] QBindable<bool> bindableInNetworkMap()
    {
        return {&mInNetworkMap};
    }

    [[nodiscard]] QBindable<bool> bindableInMagicSock()
    {
        return {&mInMagicSock};
    }

    [[nodiscard]] QBindable<bool> bindableInEngine()
    {
        return {&mInEngine};
    }

    [[nodiscard]] QBindable<bool> bindableExpired()
    {
        return {&mExpired};
    }

    [[nodiscard]] QBindable<QDateTime> bindableKeyExpiry()
    {
        return {&mKeyExpiry};
    }

    [[nodiscard]] QBindable<Location *> bindableLocation()
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
