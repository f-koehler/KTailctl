#ifndef KTAILCTL_PEER_STATUS_HPP
#define KTAILCTL_PEER_STATUS_HPP

#include "location.hpp"

#include <QBindable>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QStringList>

// https://pkg.go.dev/tailscale.com/ipn/ipnstate#PeerStatus
class PeerStatus : public QObject
{
    Q_OBJECT

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

    Q_PROPERTY(QString id READ id BINDABLE bindableId)
    Q_PROPERTY(QString publicKey READ publicKey BINDABLE bindablePublicKey)
    Q_PROPERTY(QString hostName READ hostName BINDABLE bindableHostName)
    Q_PROPERTY(QString dnsName READ dnsName BINDABLE bindableDnsName)
    Q_PROPERTY(QString os READ os BINDABLE bindableOs)
    Q_PROPERTY(qint64 userId READ userId BINDABLE bindableUserId)
    Q_PROPERTY(qint64 sharerUserId READ sharerUserId BINDABLE bindableSharerUserId)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps BINDABLE bindableTailscaleIps)
    Q_PROPERTY(QStringList allowedIps READ allowedIps BINDABLE bindableAllowedIps)
    Q_PROPERTY(QStringList tags READ tags BINDABLE bindableTags)
    Q_PROPERTY(bool mullvadNode READ mullvadNode BINDABLE bindableMullvadNode)
    Q_PROPERTY(QStringList primaryRoutes READ primaryRoutes BINDABLE bindablePrimaryRoutes)
    Q_PROPERTY(QStringList addresses READ addresses BINDABLE bindableAddresses)
    Q_PROPERTY(QString currentAddress READ currentAddress BINDABLE bindableCurrentAddress)
    Q_PROPERTY(QString relay READ relay BINDABLE bindableRelay)
    Q_PROPERTY(QString peerRelay READ peerRelay BINDABLE bindablePeerRelay)
    Q_PROPERTY(qint64 receivedBytes READ receivedBytes BINDABLE bindableReceivedBytes)
    Q_PROPERTY(qint64 transmittedBytes READ transmittedBytes BINDABLE bindableTransmittedBytes)
    Q_PROPERTY(QDateTime created READ created BINDABLE bindableCreated)
    Q_PROPERTY(QDateTime lastWrite READ lastWrite BINDABLE bindableLastWrite)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen BINDABLE bindableLastSeen)
    Q_PROPERTY(QDateTime lastHandshake READ lastHandshake BINDABLE bindableLastHandshake)
    Q_PROPERTY(bool online READ online BINDABLE bindableOnline)
    Q_PROPERTY(bool exitNode READ exitNode BINDABLE bindableExitNode)
    Q_PROPERTY(bool exitNodeOption READ exitNodeOption BINDABLE bindableExitNodeOption)
    Q_PROPERTY(bool active READ active BINDABLE bindableActive)
    Q_PROPERTY(QStringList peerApiUrls READ peerApiUrls BINDABLE bindablePeerApiUrls)
    Q_PROPERTY(TaildropTargetStatus taildropTargetStatus READ taildropTargetStatus BINDABLE bindableTaildropTargetStatus)
    Q_PROPERTY(QString noFileSharingReason READ noFileSharingReason BINDABLE bindableNoFileSharingReason)
    Q_PROPERTY(QStringList sshHostKeys READ sshHostKeys BINDABLE bindableSshHostKeys)
    Q_PROPERTY(bool shareeNode READ shareeNode BINDABLE bindableShareeNode)
    Q_PROPERTY(bool inNetworkMap READ inNetworkMap BINDABLE bindableInNetworkMap)
    Q_PROPERTY(bool inMagicSock READ inMagicSock BINDABLE bindableInMagicSock)
    Q_PROPERTY(bool inEngine READ inEngine BINDABLE bindableInEngine)
    Q_PROPERTY(bool expired READ expired BINDABLE bindableExpired)
    Q_PROPERTY(QDateTime keyExpiry READ keyExpiry BINDABLE bindableKeyExpiry)
    Q_PROPERTY(Location *location READ location BINDABLE bindableLocation)

public:
private:
    QProperty<QString> mId;
    QProperty<QString> mPublicKey;
    QProperty<QString> mHostName;
    QProperty<QString> mDnsName;
    QProperty<QString> mOs; // TODO(fk): replace with enum?
    QProperty<qint64> mUserId;
    QProperty<qint64> mSharerUserId;
    QProperty<QStringList> mTailscaleIps;
    QProperty<QStringList> mAllowedIps;
    QProperty<QStringList> mTags;
    QProperty<bool> mMullvadNode;
    QProperty<QStringList> mPrimaryRoutes;
    QProperty<QStringList> mAddresses;
    QProperty<QString> mCurrentAddress;
    QProperty<QString> mRelay;
    QProperty<QString> mPeerRelay;
    QProperty<qint64> mReceivedBytes;
    QProperty<qint64> mTransmittedBytes;
    QProperty<QDateTime> mCreated;
    QProperty<QDateTime> mLastWrite;
    QProperty<QDateTime> mLastSeen;
    QProperty<QDateTime> mLastHandshake;
    QProperty<bool> mOnline;
    QProperty<bool> mExitNode;
    QProperty<bool> mExitNodeOption;
    QProperty<bool> mActive;
    QProperty<QStringList> mPeerApiUrls;
    QProperty<TaildropTargetStatus> mTaildropTargetStatus;
    QProperty<QString> mNoFileSharingReason;
    // TODO(fk): capmap
    QProperty<QStringList> mSshHostKeys;
    QProperty<bool> mShareeNode;
    QProperty<bool> mInNetworkMap;
    QProperty<bool> mInMagicSock;
    QProperty<bool> mInEngine;
    QProperty<bool> mExpired;
    QProperty<QDateTime> mKeyExpiry;
    QProperty<Location *> mLocation;

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

    void updateFromJson(QJsonObject &json)
    {
        mId = json.take(QStringLiteral("ID")).toString();
        mPublicKey = json.take(QStringLiteral("PublicKey")).toString();
        mHostName = json.take(QStringLiteral("HostName")).toString();
        mDnsName = json.take(QStringLiteral("DNSName")).toString();
        mOs = json.take(QStringLiteral("OS")).toString();
        mUserId = json.take(QStringLiteral("UserId")).toInteger();
        mSharerUserId = json.take(QStringLiteral("AltSharerUserId")).toInteger();
        mTailscaleIps = json.take(QStringLiteral("TailscaleIPs")).toVariant().toStringList();
        mAllowedIps = json.take(QStringLiteral("AllowedIPs")).toVariant().toStringList();
        mTags = json.take(QStringLiteral("Tags")).toVariant().toStringList();
        mMullvadNode = mTags.value().contains(QStringLiteral("tag:mullvad-exit-node"));
        mPrimaryRoutes = json.take(QStringLiteral("PrimaryRoutes")).toVariant().toStringList();
        mAddresses = json.take(QStringLiteral("Addresses")).toVariant().toStringList();
        mCurrentAddress = json.take(QStringLiteral("CurrentAddress")).toString();
        mRelay = json.take(QStringLiteral("Relay")).toString();
        mPeerRelay = json.take(QStringLiteral("PeerRelay")).toString();
        mReceivedBytes = json.take(QStringLiteral("ReceivedBytes")).toInteger();
        mTransmittedBytes = json.take(QStringLiteral("TransmittedBytes")).toInteger();
        // mCreated = json.take(QStringLiteral("Created")).toString()
        // mLastWrite = json.take(QStringLiteral("LastWrite")).toString()
        // mLastSeen = json.take(QStringLiteral("LastSeen")).toString()
        // mLastHandshake = json.take(QStringLiteral("LastHandshake")).toString()
        mOnline = json.take(QStringLiteral("Online")).toBool();
        mExitNode = json.take(QStringLiteral("ExitNode")).toBool();
        mExitNodeOption = json.take(QStringLiteral("ExitNodeOption")).toBool();
        mActive = json.take(QStringLiteral("Active")).toBool();
        mPeerApiUrls = json.take(QStringLiteral("PeerApiUrls")).toVariant().toStringList();
        // mTaildropTargetStatus
        mNoFileSharingReason = json.take(QStringLiteral("NoFileSharingReason")).toString();
        mSshHostKeys = json.take(QStringLiteral("sshHostKeys")).toVariant().toStringList();
        mShareeNode = json.take(QStringLiteral("ShareeNode")).toBool();
        mInNetworkMap = json.take(QStringLiteral("InNetworkMap")).toVariant().toBool();
        mInMagicSock = json.take(QStringLiteral("InMagicSock")).toBool();
        mInEngine = json.take(QStringLiteral("InEngine")).toBool();
        mExpired = json.take(QStringLiteral("Expired")).toBool();
        // mKeyExpiry = json.take(QStringLiteral("KeyExpiry")).toString()

        if (json.contains(QStringLiteral("Location"))) {
            auto locationJson = json.take(QStringLiteral("Location")).toObject();
            if (mLocation.value() == nullptr) {
                mLocation = new Location(this);
            }
            mLocation->updateFromJson(locationJson);
        } else {
            if (mLocation.value() != nullptr) {
                mLocation->deleteLater();
                mLocation = nullptr;
            }
        }
    }

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
};

#endif // KTAILCTL_PEER_STATUS_HPP
