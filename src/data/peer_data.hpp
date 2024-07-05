#ifndef KTAILCTL_DATA_PEER_DATA_HPP
#define KTAILCTL_DATA_PEER_DATA_HPP

#include <optional>

#include <QDateTime>
#include <QLoggingCategory>
#include <QVariant>

#include "json.hpp"

Q_DECLARE_LOGGING_CATEGORY(logcat_peer_data)

struct PeerData {
    Q_GADGET
    Q_PROPERTY(QString tailscaleID MEMBER mId)
    Q_PROPERTY(QString publicKey MEMBER mPublicKey)
    Q_PROPERTY(QString hostName MEMBER mHostName)
    Q_PROPERTY(QString dnsName MEMBER mDnsName)
    Q_PROPERTY(QString os MEMBER mOs)
    Q_PROPERTY(QStringList tailscaleIps MEMBER mTailscaleIps)
    Q_PROPERTY(QString relay MEMBER mRelay)
    Q_PROPERTY(long rxBytes MEMBER mRxBytes)
    Q_PROPERTY(long txBytes MEMBER mTxBytes)
    Q_PROPERTY(QString created MEMBER mCreated)
    Q_PROPERTY(QString lastSeen MEMBER mLastSeen)
    Q_PROPERTY(bool isOnline MEMBER mIsOnline)
    Q_PROPERTY(bool isActive MEMBER mIsActive)
    Q_PROPERTY(bool isCurrentExitNode MEMBER mIsCurrentExitNode)
    Q_PROPERTY(bool isExitNode MEMBER mIsExitNode)
    Q_PROPERTY(QStringList sshHostKeys MEMBER mSshHostKeys)
    Q_PROPERTY(QStringList tags MEMBER mTags)
    Q_PROPERTY(bool isMullvad MEMBER mIsMullvad)
    Q_PROPERTY(QString country MEMBER mCountry)
    Q_PROPERTY(QString countryCode MEMBER mCountryCode)
    Q_PROPERTY(QString city MEMBER mCity)
    Q_PROPERTY(QString cityCode MEMBER mCityCode)

public:
    QString mId;
    QString mPublicKey;
    QString mHostName;
    QString mDnsName;
    QString mOs;
    QStringList mTailscaleIps;
    QString mRelay;
    long mRxBytes;
    long mTxBytes;
    QString mCreated;
    QString mLastSeen;
    bool mIsOnline;
    bool mIsActive;
    bool mIsCurrentExitNode;
    bool mIsExitNode;
    QStringList mSshHostKeys;
    QStringList mTags;
    bool mIsMullvad;
    QString mCountry;
    QString mCountryCode;
    QString mCity;
    QString mCityCode;

    bool operator==(const PeerData &other) const = default;
    bool operator!=(const PeerData &other) const = default;

    Q_INVOKABLE bool isRunningSSH() const;
    Q_INVOKABLE QString sshCommand() const;
};
Q_DECLARE_METATYPE(PeerData)

void from_json(const json &j, PeerData &p);
void from_json(const json &j, QVector<PeerData> &peers);

#endif /* KTAILCTL_DATA_PEER_DATA_HPP */
