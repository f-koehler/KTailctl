#include "peer_status.hpp"

#include <QJsonValue>
#include <QStringLiteral>
#include <QVariant>
#include <Qt>
#include <utility>

#include "location.hpp"

void PeerStatus::updateFromJson(QJsonObject &json)
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
    mCreated = QDateTime::fromString(json.take(QStringLiteral("Created")).toString(), Qt::ISODateWithMs);
    mLastWrite = QDateTime::fromString(json.take(QStringLiteral("LastWrite")).toString(), Qt::ISODateWithMs);
    const QString lastSeenStr = json.take(QStringLiteral("LastSeen")).toString();
    mLastSeen = lastSeenStr == QStringLiteral("0001-01-01T00:00:00Z") ? QDateTime::currentDateTimeUtc() : QDateTime::fromString(lastSeenStr, Qt::ISODateWithMs);
    mLastHandshake = QDateTime::fromString(json.take(QStringLiteral("LastHandshake")).toString(), Qt::ISODateWithMs);
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
