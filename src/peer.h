// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_PEER_H
#define TAILCTL_PEER_H

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QString>
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
    Q_PROPERTY(QVector<QString> tailscaleIPs READ tailscaleIPs NOTIFY tailscaleIPsChanged)
    Q_PROPERTY(QString relay READ relay NOTIFY relayChanged)
    Q_PROPERTY(long rxBytes READ rxBytes NOTIFY rxBytesChanged)
    Q_PROPERTY(long txBytes READ txBytes NOTIFY txBytesChanged)
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY isOnlineChanged)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

private:
    QString m_id;
    QString m_public_key;
    QString m_host_name;
    QString m_dns_name;
    QString m_os;
    QVector<QString> m_tailscale_ips;
    QString m_relay;
    long m_rx_bytes;
    long m_tx_bytes;
    bool m_is_online;
    bool m_is_active;

signals:
    void idChanged(const QString &);
    void publicKeyChanged(const QString &);
    void hostNameChanged(const QString &);
    void dnsNameChanged(const QString &);
    void osChanged(const QString &);
    void tailscaleIPsChanged(const QVector<QString> &);
    void relayChanged(const QString &);
    void rxBytesChanged(long);
    void txBytesChanged(long);
    void isOnlineChanged(bool);
    void isActiveChanged(bool);

public slots:
    void updateFromStatus(const Status &status);

public:
    Peer(QObject *parent = nullptr);
    virtual ~Peer() = default;

    static Peer *fromJSON(const QJsonObject &json);
    void read(const QJsonObject &json);

    const QString &id() const;
    const QString &publicKey() const;
    const QString &hostName() const;
    const QString &dnsName() const;
    const QString &os() const;
    const QVector<QString> &tailscaleIPs() const;
    const QString &relay() const;
    long rxBytes() const;
    long txBytes() const;
    bool isOnline() const;
    bool isActive() const;

    Peer &operator=(const Peer &other);
};

#endif // TAILCTL_PEER_H
