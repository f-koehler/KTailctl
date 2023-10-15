// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_PEER_H
#define KTAILCTL_PEER_H

#include <QDateTime>
#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QStringList>
#include <QVector>

class Status;

class Peer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString tailscaleID READ id NOTIFY idChanged)
    Q_PROPERTY(QString publicKey READ publicKey NOTIFY publicKeyChanged)
    Q_PROPERTY(QString hostName READ hostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString dnsName READ dnsName NOTIFY dnsNameChanged)
    Q_PROPERTY(QString os READ os NOTIFY osChanged)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps NOTIFY tailscaleIpsChanged)
    Q_PROPERTY(QString relay READ relay NOTIFY relayChanged)
    Q_PROPERTY(long rxBytes READ rxBytes NOTIFY rxBytesChanged)
    Q_PROPERTY(long txBytes READ txBytes NOTIFY txBytesChanged)
    Q_PROPERTY(QDateTime created READ created NOTIFY createdChanged)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen NOTIFY lastSeenChanged)
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY isOnlineChanged)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)
    Q_PROPERTY(bool isCurrentExitNode READ isCurrentExitNode NOTIFY isCurrentExitNodeChanged)
    Q_PROPERTY(bool isExitNode READ isExitNode NOTIFY isExitNodeChanged)
    Q_PROPERTY(QStringList sshHostKeys READ sshHostKeys NOTIFY sshHostKeysChanged)
    Q_PROPERTY(bool isRunningSSH READ isRunningSSH NOTIFY isRunningSSHChanged)
    Q_PROPERTY(QStringList tags READ tags NOTIFY tagsChanged)
    Q_PROPERTY(bool isMullvad READ isMullvad NOTIFY isMullvadChanged)

private:
    QString mId;
    QString mPublicKey;
    QString mHostName;
    QString mDNSName;
    QString mOs;
    QStringList mTailscaleIps;
    QString mRelay;
    long mRxBytes{};
    long mTxBytes{};
    QDateTime mCreated;
    QDateTime mLastSeen;
    bool mIsOnline{};
    bool mIsActive{};
    bool mIsCurrentExitNode{};
    bool mIsExitNode{};
    QStringList mSSHHostKeys{};
    bool mIsRunningSSH;
    QStringList mTags{};
    bool mIsMullvad;

protected:
    bool setId(const QString &value);
    bool setPublicKey(const QString &value);
    bool setHostName(const QString &value);
    bool setDNSName(const QString &value);
    bool setOs(const QString &value);
    bool setTailscaleIps(const QStringList &value);
    bool setRelay(const QString &value);
    bool setRxBytes(long value);
    bool setTxBytes(long value);
    bool setCreated(const QDateTime &value);
    bool setLastSeen(const QDateTime &value);
    bool setIsOnline(bool value);
    bool setIsActive(bool value);
    bool setIsCurrentExitNode(bool value);
    bool setIsExitNode(bool value);
    bool setSSHHostKeys(const QStringList &value);
    bool setIsRunningSSH(bool value);
    bool setTags(const QStringList &value);
    bool setIsMullvad(bool value);

    void setIdFromJSON(const QJsonObject &json);
    void setPublicKeyFromJSON(const QJsonObject &json);
    void setHostNameFromJSON(const QJsonObject &json);
    void setDNSNameFromJSON(const QJsonObject &json);
    void setOsFromJSON(const QJsonObject &json);
    void setTailscaleIpsFromJSON(const QJsonObject &json);
    void setRelayFromJSON(const QJsonObject &json);
    void setRxBytesFromJSON(const QJsonObject &json);
    void setTxBytesFromJSON(const QJsonObject &json);
    void setCreatedFromJSON(const QJsonObject &json);
    void setLastSeenFromJSON(const QJsonObject &json);
    void setIsOnlineFromJSON(const QJsonObject &json);
    void setIsActiveFromJSON(const QJsonObject &json);
    void setIsCurrentExitNodeFromJson(const QJsonObject &object);
    void setIsExitNodeFromJSON(const QJsonObject &json);
    void setSSHHostKeysFromJSON(const QJsonObject &json);
    void setTagsFromJSON(const QJsonObject &json);

signals:
    void idChanged(const QString &);
    void publicKeyChanged(const QString &);
    void hostNameChanged(const QString &);
    void dnsNameChanged(const QString &);
    void osChanged(const QString &);
    void tailscaleIpsChanged(const QStringList &);
    void relayChanged(const QString &);
    void rxBytesChanged(long);
    void txBytesChanged(long);
    void createdChanged(const QDateTime &);
    void lastSeenChanged(const QDateTime &);
    void isOnlineChanged(bool);
    void isActiveChanged(bool);
    void isCurrentExitNodeChanged(bool);
    void isExitNodeChanged(bool);
    void sshHostKeysChanged(const QStringList &);
    void isRunningSSHChanged(bool);
    void tagsChanged(const QStringList &);
    void isMullvadChanged(bool);

public slots:
    Q_INVOKABLE void updateFromStatus(const Status &status);

public:
    explicit Peer(QObject *parent = nullptr);
    virtual ~Peer() = default;

    static Peer *fromJSON(const QJsonObject &json, QObject *parent = nullptr);
    bool setTo(const Peer *other);
    Q_INVOKABLE QString getSSHCommand() const;

    const QString &id() const;
    const QString &publicKey() const;
    const QString &hostName() const;
    const QString &dnsName() const;
    const QString &os() const;
    const QStringList &tailscaleIps() const;
    const QString &relay() const;
    long rxBytes() const;
    long txBytes() const;
    const QDateTime &created() const;
    const QDateTime &lastSeen() const;
    bool isOnline() const;
    bool isActive() const;
    bool isCurrentExitNode() const;
    bool isExitNode() const;
    const QStringList &sshHostKeys() const;
    bool isRunningSSH() const;
    const QStringList &tags() const;
    bool isMullvad() const;

    Peer &operator=(const Peer &other);
};

#endif /* KTAILCTL_PEER_H */
