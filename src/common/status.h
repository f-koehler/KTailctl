#ifndef KTAILCTL_STATUS_H
#define KTAILCTL_STATUS_H

#include <tuple>

#include "json.h"
#include "peer.h"
#include "status_data.h"

class Status : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool success READ success NOTIFY successChanged)
    Q_PROPERTY(bool isOperator READ isOperator NOTIFY isOperatorChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool isTun READ isTun NOTIFY isTunChanged)
    Q_PROPERTY(QString backendState READ backendState NOTIFY backendStateChanged)
    Q_PROPERTY(Peer *self READ self NOTIFY selfChanged)
    Q_PROPERTY(QVector<Peer *> peers READ peers NOTIFY peersChanged)
    Q_PROPERTY(Peer *currentExitNode READ currentExitNode NOTIFY currentExitNodeChanged)
    Q_PROPERTY(Peer *suggestedExitNode READ suggestedExitNode NOTIFY suggestedExitNodeChanged)

private:
    bool mSuccess = false;
    bool mIsOperator = false;
    StatusData mData;
    Peer *mSelf = nullptr;
    QVector<Peer *> mPeers;
    Peer *mCurrentExitNode;
    Peer *mSuggestedExitNode = nullptr;

    StatusData mNewData;

public:
    explicit Status(QObject *parent = nullptr);
    virtual ~Status() = default;

    bool success() const;
    bool isOperator() const;
    const QString &version() const;
    bool isTun() const;
    const QString &backendState() const;
    Peer *self() const;
    QVector<Peer *> &peers();
    const QVector<Peer *> &peers() const;
    Peer *currentExitNode();
    Peer *suggestedExitNode() const;

    const StatusData &statusData() const;

    Q_INVOKABLE std::tuple<QList<Peer *>, QList<Peer *>> exitNodes() const;

signals:
    void successChanged(bool);
    void isOperatorChanged(bool);
    void versionChanged(const QString &);
    void isTunChanged(bool);
    void backendStateChanged(const QString &);
    void selfChanged(const Peer *);
    void peersChanged(const QVector<Peer *> &);
    void currentExitNodeChanged(Peer *);
    void suggestedExitNodeChanged(const Peer *);

    void refreshed(const Status &);

public slots:
    void update(StatusData &newData);
    Q_INVOKABLE void refresh();
};

#endif /* KTAILCTL_STATUS_H */
