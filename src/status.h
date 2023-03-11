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

    Q_PROPERTY(QString version READ version)
    Q_PROPERTY(bool tun READ isTUN)
    Q_PROPERTY(QString backendState READ backendState)
    Q_PROPERTY(QVector<Peer *> peers READ peers)

private:
    QString m_version;
    bool m_is_tun;
    QString m_backend_state;
    Peer *m_self;
    QVector<Peer *> m_peers;

public:
    void refresh(const QString &executable = QStringLiteral("tailscale"));
    void read(const QJsonObject &json);

    const QString &version() const;
    bool isTUN() const;
    const QString &backendState() const;
    const Peer *self() const;
    const QVector<Peer *> &peers() const;
};

#endif /* TAILCTL_STATUS_H */
