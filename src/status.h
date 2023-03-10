// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_STATUS_H
#define TAILCTL_STATUS_H

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QList>

struct Peer {
    Q_GADGET

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
    void read(const QJsonObject& json);

    const QString& getID() const;
    const QString& getPublicKey() const;
    const QString& getHostName() const;
    const QString& getDNSName() const;
    const QString& getOS() const;
    const QVector<QString>& getTailscaleIPs() const;
    bool isOnline() const;
    bool isActive() const;
};

struct Status {
    Q_GADGET

private:
  QString m_version;
  bool m_tun;
  QString m_backend_state;
  Peer m_self;
  QVector<Peer> m_peers;

public:
    void refresh(const QString& executable = QStringLiteral("tailscale"));
    void read(const QJsonObject& json);

    const QString& getVersion() const;
    bool isTUN() const;
    const QString& getBackendState() const;
    const Peer& getSelf() const;
    const QVector<Peer> & getPeers() const;
};

#endif /* TAILCTL_STATUS_H */
