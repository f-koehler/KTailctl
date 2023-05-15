// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_PEER_H
#define TAILCTL_PEER_H

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

private:
    QString mId;
    QString mPublicKey;
    QString mHostName;
    QString mDNSName;
    QString mOs;
    QStringList mTailscaleIps;
    QString mRelay;
    long mRxBytes;
    long mTxBytes;
    QDateTime mCreated;
    QDateTime mLastSeen;
    bool mIsOnline;
    bool mIsActive;

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

public slots:
    Q_INVOKABLE void updateFromStatus(const Status &status);

public:
    explicit Peer(QObject *parent = nullptr);
    virtual ~Peer() = default;

    static Peer *fromJSON(const QJsonObject &json);
    bool setTo(const Peer *other);

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

    Peer &operator=(const Peer &other);
};

#endif // TAILCTL_PEER_H
