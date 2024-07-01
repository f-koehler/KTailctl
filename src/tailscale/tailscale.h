#ifndef KTAILCTL_TAILSCALE_TAILSCALE_H
#define KTAILCTL_TAILSCALE_TAILSCALE_H

#include <QObject>

#include "peer.h"
#include "peer_model.h"
#include "status_data.h"

class Tailscale : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PeerModel *peerModel READ peerModel CONSTANT)
    Q_PROPERTY(bool success READ success NOTIFY successChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool isOperator READ isOperator NOTIFY isOperatorChanged)
    Q_PROPERTY(QString backendState READ backendState NOTIFY backendStateChanged)
    Q_PROPERTY(Peer *self READ self CONSTANT)
    Q_PROPERTY(QVector<Peer *> peers READ peers NOTIFY peersChanged)
    Q_PROPERTY(QVector<Peer *> exitNodes READ exitNodes NOTIFY exitNodesChanged)
    Q_PROPERTY(Peer *exitNode READ exitNode NOTIFY exitNodeChanged)
    Q_PROPERTY(Peer *suggestedExitNode READ suggestedExitNode NOTIFY suggestedExitNodeChanged)

private:
    PeerModel *mPeerModel = nullptr;
    PeerModel *mExitNodeModel = nullptr;

    bool mSuccess = false;
    QString mVersion;
    bool mIsOperator = false;
    QString mBackendState;
    Peer *mSelf = nullptr;
    QVector<Peer *> mPeers;
    QVector<Peer *> mExitNodes;
    Peer *mExitNode = nullptr;
    Peer *mSuggestedExitNode = nullptr;

    explicit Tailscale(QObject *parent = nullptr);

signals:
    void successChanged(bool);
    void versionChanged(const QString &);
    void isOperatorChanged(bool);
    void backendStateChanged(const QString &);
    void peersChanged(const QVector<Peer *> &);
    void exitNodesChanged(const QVector<Peer *> &);
    void exitNodeChanged(Peer *);
    void suggestedExitNodeChanged(Peer *);

public:
    static Tailscale *instance();
    virtual ~Tailscale() = default;

    PeerModel *peerModel() const;
    bool success() const;
    const QString &version() const;
    bool isOperator() const;
    const QString &backendState() const;
    Peer *self() const;
    const QVector<Peer *> &peers() const;
    const QVector<Peer *> &exitNodes() const;
    Peer *exitNode() const;
    Peer *suggestedExitNode() const;

public slots:
    Q_INVOKABLE void up();
    Q_INVOKABLE void down();
    Q_INVOKABLE void toggle();
    Q_INVOKABLE void refresh();
};

#endif /* KTAILCTL_TAILSCALE_TAILSCALE_H */
