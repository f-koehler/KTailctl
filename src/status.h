// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_STATUS_H
#define KTAILCTL_STATUS_H

#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QVector>

#include "peer.h"
#include <libtailwrap.h>

class Status : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool success READ success NOTIFY successChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool isTUN READ isTUN NOTIFY isTUNChanged)
    Q_PROPERTY(QString backendState READ backendState NOTIFY backendStateChanged)
    Q_PROPERTY(QVector<Peer *> peers READ peers NOTIFY peersChanged)
    // Q_PROPERTY(Peer *self READ self NOTIFY selfChanged)
    Q_PROPERTY(bool isOperator READ isOperator NOTIFY isOperatorChanged)
    Q_PROPERTY(QStringList exitNodes READ exitNodes NOTIFY exitNodesChanged)

private:
    bool mSuccess{};
    QString mVersion;
    bool mIsTun{};
    QString mBackendState;
    Peer *mSelf;
    QVector<Peer *> mPeers;
    GoString mStatusBuffer{};
    bool mIsOperator{};
    QStringList mExitNodes;

signals:
    void successChanged(bool);
    void versionChanged(const QString &);
    void isTUNChanged(bool);
    void backendStateChanged(const QString &);
    void peersChanged(const QVector<Peer *> &);
    void selfChanged(const Peer *);
    void refreshed(const Status &);
    void isOperatorChanged(bool);
    void exitNodesChanged(const QStringList &);

public:
    Status(QObject *parent = nullptr);
    virtual ~Status() = default;

    Q_INVOKABLE void refresh();
    void read(const QJsonObject &json);

    bool success() const;
    const QString &version() const;
    bool isTUN() const;
    const QString &backendState() const;
    const Peer *self() const;
    const QVector<Peer *> &peers() const;
    bool isOperator() const;
    const QStringList &exitNodes() const;
};

#endif /* KTAILCTL_STATUS_H */
