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

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString publicKey READ publicKey)
    Q_PROPERTY(QString hostName READ hostName)
    Q_PROPERTY(QString dnsName READ dnsName)
    Q_PROPERTY(QString os READ os)
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
    bool m_is_online;
    bool m_is_active;

public:
    void read(const QJsonObject &json);

    const QString &id() const;
    const QString &publicKey() const;
    const QString &hostName() const;
    const QString &dnsName() const;
    const QString &os() const;
    const QVector<QString> &tailscaleIPs() const;
    bool isOnline() const;
    bool isActive() const;
};

#endif // TAILCTL_PEER_H
