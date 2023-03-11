// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_STATUS_H
#define TAILCTL_STATUS_H

#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <QVector>

#include "peer.h"

struct Status : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString version READ getVersion MEMBER m_version NOTIFY versionChanged)
    Q_PROPERTY(bool tun READ isTUN MEMBER m_tun NOTIFY tunChanged)
    Q_PROPERTY(QString backendState READ getBackendState MEMBER m_backend_state NOTIFY backendStateChanged)

private:
    QString m_version;
    bool m_tun;
    QString m_backend_state;
    Peer *m_self;
    QVector<Peer *> m_peers;

signals:
    void versionChanged(const QString &);
    void tunChanged(bool);
    void backendStateChanged(const QString &);

public:
    void refresh(const QString &executable = QStringLiteral("tailscale"));
    void read(const QJsonObject &json);

    const QString &getVersion() const;
    bool isTUN() const;
    const QString &getBackendState() const;
    const Peer *getSelf() const;
    const QVector<Peer *> &getPeers() const;
};

#endif /* TAILCTL_STATUS_H */
