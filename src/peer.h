// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_PEER_H
#define TAILCTL_PEER_H

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVector>

struct Peer : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString id READ getID MEMBER m_id NOTIFY idChanged)
    Q_PROPERTY(QString publicKey READ getPublicKey MEMBER m_public_key NOTIFY publicKeyChanged)
    Q_PROPERTY(QString hostName READ getHostName MEMBER m_host_name NOTIFY hostNameChanged)
    Q_PROPERTY(QString dnsName READ getDNSName MEMBER m_dns_name NOTIFY dnsNameChanged)
    Q_PROPERTY(QString os READ getOS MEMBER m_os NOTIFY osChanged)
    Q_PROPERTY(QVector<QString> tailscaleIPs READ getTailscaleIPs MEMBER m_tailscale_ips NOTIFY tailscaleIPsChanged)
    Q_PROPERTY(bool online READ isOnline MEMBER m_online NOTIFY onlineChanged)
    Q_PROPERTY(bool active READ isActive MEMBER m_active NOTIFY activeChanged)

    QML_ELEMENT

private:
    QString m_id;
    QString m_public_key;
    QString m_host_name;
    QString m_dns_name;
    QString m_os;
    QVector<QString> m_tailscale_ips;
    bool m_online;
    bool m_active;

signals:
    void idChanged(const QString &);
    void publicKeyChanged(const QString &);
    void hostNameChanged(const QString &);
    void dnsNameChanged(const QString &);
    void osChanged(const QString &);
    void tailscaleIPsChanged(const QVector<QString> &);
    bool onlineChanged(bool);
    bool activeChanged(bool);

public:
    void read(const QJsonObject &json);

    const QString &getID() const;
    const QString &getPublicKey() const;
    const QString &getHostName() const;
    const QString &getDNSName() const;
    const QString &getOS() const;
    const QVector<QString> &getTailscaleIPs() const;
    bool isOnline() const;
    bool isActive() const;
};

#endif // TAILCTL_PEER_H
