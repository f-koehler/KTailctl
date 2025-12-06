//
// Created by fkoehler on 12/6/25.
//

#ifndef KTAILCTL_PEER_H
#define KTAILCTL_PEER_H

#include <QBindable>
#include <QObject>
#include <QProperty>
#include <QString>
#include <QStringList>

class Peer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId BINDABLE bindableId)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName BINDABLE bindableHostName)
    Q_PROPERTY(QString dnsName READ dnsName WRITE setDnsName BINDABLE bindableDnsName)
    Q_PROPERTY(QString os READ os WRITE setOs BINDABLE bindableOs)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps BINDABLE bindableTailscaleIps)
    Q_PROPERTY(QString relay READ relay WRITE setRelay BINDABLE bindableRelay)
    Q_PROPERTY(long receivedBytes READ receivedBytes WRITE setReceivedBytes BINDABLE bindableReceivedBytes)
    Q_PROPERTY(long transmittedBytes WRITE transmittedBytes WRITE setTransmittedBytes BINDABLE bindableTransmittedBytes)
    Q_PROPERTY(QDateTime created READ created WRITE setCreated BINDABLE bindableCreated)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen WRITE setLastSeen BINDABLE bindableLastSeen)
    Q_PROPERTY(bool isOnline READ isOnline WRITE setIsOnline BINDABLE bindableIsOnline)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive BINDABLE bindableIsActive)
    Q_PROPERTY(bool isCurrentExitNode READ isCurrentExitNode WRITE setIsCurrentExitNode BINDABLE bindableIsCurrentExitNode)
    Q_PROPERTY(bool isExitNode READ isExitNode WRITE setIsExitNode BINDABLE bindableIsExitNode)
    Q_PROPERTY(QStringList sshHostKeys READ sshHostKeys WRITE setSshHostKeys BINDABLE bindableSshHostKeys)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags BINDABLE bindableTags)
    Q_PROPERTY(bool isMullvad READ isMullvad WRITE setIsMullvad BINDABLE bindableIsMullvad)
    Q_PROPERTY(QString country READ country WRITE setCountry BINDABLE bindableCountry)
    Q_PROPERTY(QString countryCode READ countryCode WRITE setCountryCode BINDABLE bindableCountryCode)
    Q_PROPERTY(QString city READ city WRITE setCity BINDABLE bindableCity)
    Q_PROPERTY(QString cityCode READ cityCode WRITE setCityCode BINDABLE bindableCityCode)

private:
    QProperty<QString> mId;
    QProperty<QString> mHostName;
    QProperty<QString> mDnsName;
    QProperty<QString> mOs;
    QProperty<QStringList> mTailscaleIps;
    QProperty<QString> mRelay;
    QProperty<long> mReceivedBytes;
    QProperty<long> mTransmittedBytes;
    QProperty<QDateTime> mCreated;
    QProperty<QDateTime> mLastSeen;
    QProperty<bool> mIsOnline;
    QProperty<bool> mIsActive;
    QProperty<bool> mIsCurrentExitNode;
    QProperty<bool> mIsExitNode;
    QProperty<QStringList> mSshHostKeys;
    QProperty<QStringList> mTags;
    QProperty<bool> mIsMullvad;
    QProperty<QString> mCountry;
    QProperty<QString> mCountryCode;
    QProperty<QString> mCity;
    QProperty<QString> mCityCode;
    QProperty<QUrl> mAdminPanelUrl;

public:
    // getters
    [[nodiscard]] const QString &id() const
    {
        return mId;
    }

    [[nodiscard]] const QString &hostName() const
    {
        return mHostName;
    }

    [[nodiscard]] const QString &dnsName() const
    {
        return mDnsName;
    }

    [[nodiscard]] const QString &os() const
    {
        return mOs;
    }

    [[nodiscard]] const QStringList &tailscaleIps() const
    {
        return mTailscaleIps;
    }

    [[nodiscard]] const QString &relay() const
    {
        return mRelay;
    }

    [[nodiscard]] long receivedBytes() const
    {
        return mReceivedBytes;
    }

    [[nodiscard]] long transmittedBytes() const
    {
        return mTransmittedBytes;
    }

    [[nodiscard]] const QDateTime &created() const
    {
        return mCreated;
    }

    [[nodiscard]] const QDateTime &lastSeen() const
    {
        return mLastSeen;
    }

    [[nodiscard]] bool isOnline() const
    {
        return mIsOnline;
    }

    [[nodiscard]] bool isActive() const
    {
        return mIsActive;
    }

    [[nodiscard]] bool isCurrentExitNode() const
    {
        return mIsCurrentExitNode;
    }

    [[nodiscard]] bool isExitNode() const
    {
        return mIsExitNode;
    }

    [[nodiscard]] const QStringList &sshHostKeys() const
    {
        return mSshHostKeys;
    }

    [[nodiscard]] const QStringList &tags() const
    {
        return mTags;
    }

    [[nodiscard]] bool isMullvad() const
    {
        return mIsMullvad;
    }

    [[nodiscard]] const QString &country() const
    {
        return mCountry;
    }

    [[nodiscard]] const QString &countryCode() const
    {
        return mCountryCode;
    }

    [[nodiscard]] const QString &city() const
    {
        return mCity;
    }

    [[nodiscard]] const QString &cityCode() const
    {
        return mCityCode;
    }

    [[nodiscard]] const QUrl &adminPanelUrl() const
    {
        return mAdminPanelUrl;
    }

    // bindables
    QBindable<QString> bindableId()
    {
        return {&mId};
    }

    QBindable<QString> bindableHostName()
    {
        return {&mHostName};
    }

    QBindable<QString> bindableDnsName()
    {
        return {&mDnsName};
    }

    QBindable<QString> bindableOs()
    {
        return {&mOs};
    }

    QBindable<QStringList> bindableTailscaleIps()
    {
        return {&mTailscaleIps};
    }

    QBindable<QString> bindableRelay()
    {
        return {&mRelay};
    }

    QBindable<long> bindableReceivedBytes()
    {
        return {&mReceivedBytes};
    }

    QBindable<long> bindableTransmittedBytes()
    {
        return {&mTransmittedBytes};
    }

    QBindable<QDateTime> bindableCreated()
    {
        return {&mCreated};
    }

    QBindable<QDateTime> bindableLastSeen()
    {
        return {&mLastSeen};
    }

    QBindable<bool> bindableIsOnline()
    {
        return {&mIsOnline};
    }

    QBindable<bool> bindableIsActive()
    {
        return {&mIsActive};
    }

    QBindable<bool> bindableIsCurrentExitNode()
    {
        return {&mIsCurrentExitNode};
    }

    QBindable<bool> bindableIsExitNode()
    {
        return {&mIsExitNode};
    }

    QBindable<QStringList> bindableSshHostKeys()
    {
        return {&mSshHostKeys};
    }

    QBindable<QStringList> bindableTags()
    {
        return {&mTags};
    }

    QBindable<bool> bindableIsMullvad()
    {
        return {&mIsMullvad};
    }

    QBindable<QString> bindableCountry()
    {
        return {&mCountry};
    }

    QBindable<QString> bindableCountryCode()
    {
        return {&mCountryCode};
    }

    QBindable<QString> bindableCity()
    {
        return {&mCity};
    }

    QBindable<QString> bindableCityCode()
    {
        return {&mCityCode};
    }

    QBindable<QString> bindableAdminPanelUrl()
    {
        return {&mAdminPanelUrl};
    }

    // setters
    void setId(const QString &id)
    {
        mId = id;
    }

    void setHostName(const QString &hostName)
    {
        mHostName = hostName;
    }

    void setDnsName(const QString &dnsName)
    {
        mDnsName = dnsName;
    }

    void setOs(const QString &os)
    {
        mOs = os;
    }

    void setTailscaleIps(const QStringList &tailscaleIps)
    {
        mTailscaleIps = tailscaleIps;
    }

    void setRelay(const QString &relay)
    {
        mRelay = relay;
    }

    void setReceivedBytes(long receivedBytes)
    {
        mReceivedBytes = receivedBytes;
    }

    void setTransmittedBytes(long transmittedBytes)
    {
        mTransmittedBytes = transmittedBytes;
    }

    void setCreated(const QDateTime &created)
    {
        mCreated = created;
    }

    void setLastSeen(const QDateTime &lastSeen)
    {
        mLastSeen = lastSeen;
    }

    void setIsOnline(bool isOnline)
    {
        mIsOnline = isOnline;
    }

    void setIsActive(bool isActive)
    {
        mIsActive = isActive;
    }

    void setIsCurrentExitNode(bool isCurrentExitNode)
    {
        mIsCurrentExitNode = isCurrentExitNode;
    }

    void setIsExitNode(bool isExitNode)
    {
        mIsExitNode = isExitNode;
    }

    void setSshHostKeys(const QStringList &sshHostKeys)
    {
        mSshHostKeys = sshHostKeys;
    }

    void setTags(const QStringList &tags)
    {
        mTags = tags;
    }

    void setIsMullvad(bool isMullvad)
    {
        mIsMullvad = isMullvad;
    }

    void setCountry(const QString &country)
    {
        mCountry = country;
    }

    void setCountryCode(const QString &countryCode)
    {
        mCountryCode = countryCode;
    }

    void setCity(const QString &city)
    {
        mCity = city;
    }

    void setCityCode(const QString &cityCode)
    {
        mCityCode = cityCode;
    }
};

#endif // KTAILCTL_PEER_H