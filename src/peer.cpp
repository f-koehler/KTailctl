// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "peer.h"

#include <QJsonArray>

Peer::Peer(QObject *parent) : QObject(parent) {}

Peer *Peer::fromJSON(const QJsonObject &json) {
  Peer *peer = new Peer();
  peer->read(json);
  return peer;
}

void Peer::read(const QJsonObject &json) {
  if (json.contains("ID") && json["ID"].isString()) {
    if (auto id = json["ID"].toString(); id != m_id) {
      m_id = id;
      emit idChanged(m_id);
    }
  } else {
    qWarning() << "Cannot find string \"ID\"";
  }

  if (json.contains("PublicKey") && json["PublicKey"].isString()) {
    if (auto public_key = json["PublicKey"].toString();
        public_key != m_public_key) {
      m_public_key = public_key;
      emit publicKeyChanged(m_public_key);
    }
  } else {
    qWarning() << "Cannot find string \"PublicKey\"";
  }

  if (json.contains("HostName") && json["HostName"].isString()) {
    if (auto host_name = json["HostName"].toString();
        host_name != m_host_name) {
      m_host_name = host_name;
      emit hostNameChanged(m_host_name);
    }
  } else {
    qWarning() << "Cannot find string \"HostName\"";
  }

  if (json.contains("DNSName") && json["DNSName"].isString()) {
    if (auto dns_name = json["DNSName"].toString(); dns_name != m_dns_name) {
      m_dns_name = json["DNSName"].toString();
      emit dnsNameChanged(m_dns_name);
    }
  } else {
    qWarning() << "Cannot find string \"DNSName\"";
  }

  if (json.contains("OS") && json["OS"].isString()) {
    if (auto os_name = json["OS"].toString(); os_name != m_os) {
      m_os = json["OS"].toString();
      emit osChanged(m_os);
    }
  } else {
    qWarning() << "Cannot find string \"OS\"";
  }

  if (json.contains("TailscaleIPs") && json["TailscaleIPs"].isArray()) {
    m_tailscale_ips.clear();
    for (const auto &ip : json["TailscaleIPs"].toArray()) {
      if (ip.isString()) {
        m_tailscale_ips.append(ip.toString());
      } else {
        qWarning() << "TailscaleIPs contains non-string";
      }
    }
  } else {
    qWarning() << "Cannot find array \"TailscaleIPs\"";
  }

  if (json.contains("Online") && json["Online"].isBool()) {
    if (const auto is_online = json["Online"].toBool();
        is_online != m_is_online) {
      m_is_online = json["Online"].toBool();
      emit isOnlineChanged(m_is_online);
    }
  } else {
    qWarning() << "Cannot find bool \"Online\"";
  }

  if (json.contains("Active") && json["Active"].isBool()) {
    if (const auto is_active = json["Active"].toBool();
        is_active != m_is_active) {
      m_is_active = json["Active"].toBool();
      emit isActiveChanged(m_is_active);
    }
  } else {
    qWarning() << "Cannot find bool \"Active\"";
  }
}

const QString &Peer::id() const { return m_id; }

const QString &Peer::publicKey() const { return m_public_key; }

const QString &Peer::hostName() const { return m_host_name; }

const QString &Peer::dnsName() const { return m_dns_name; }

const QString &Peer::os() const { return m_os; }

const QVector<QString> &Peer::tailscaleIPs() const { return m_tailscale_ips; }

bool Peer::isOnline() const { return m_is_online; }

bool Peer::isActive() const { return m_is_active; }
