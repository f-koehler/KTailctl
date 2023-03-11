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

    Q_PROPERTY(QString id READ getID)
    Q_PROPERTY(QString publicKey READ getPublicKey)
    Q_PROPERTY(QString hostName READ getHostName)
    Q_PROPERTY(QString dnsName READ getDNSName)
    Q_PROPERTY(QString os READ getOS)
    Q_PROPERTY(QVector<QString> tailscaleIPs)
    Q_PROPERTY(bool online READ isOnline)
    Q_PROPERTY(bool active READ isActive)

private:
    QString m_id;
    QString m_public_key;
    QString m_host_name;
    QString m_dns_name;
    QString m_os;
    QVector<QString> m_tailscale_ips;
    bool m_online;
    bool m_active;

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
