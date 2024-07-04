#ifndef KTAILCTL_TAILSCALE_TAILSCALE_H
#define KTAILCTL_TAILSCALE_TAILSCALE_H

#include <QObject>

#include "exit_node_model.h"
#include "mullvad_country_model.h"
#include "mullvad_node_model.h"
#include "peer_model.h"
#include "status_data.h"

class Tailscale : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PeerModel *peerModel READ peerModel CONSTANT)
    Q_PROPERTY(ExitNodeModel *exitNodeModel READ exitNodeModel CONSTANT)
    Q_PROPERTY(MullvadNodeModel *mullvadNodeModel READ mullvadNodeModel CONSTANT)
    Q_PROPERTY(MullvadCountryModel *mullvadCountryModel READ mullvadCountryModel CONSTANT)
    Q_PROPERTY(bool success READ success NOTIFY successChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool isOperator READ isOperator NOTIFY isOperatorChanged)
    Q_PROPERTY(QString backendState READ backendState NOTIFY backendStateChanged)
    Q_PROPERTY(PeerData self READ self NOTIFY selfChanged)

private:
    PeerModel *mPeerModel = nullptr;
    ExitNodeModel *mExitNodeModel = nullptr;
    MullvadNodeModel *mMullvadNodeModel = nullptr;
    MullvadCountryModel *mMullvadCountryModel = nullptr;

    bool mSuccess = false;
    QString mVersion;
    bool mIsOperator = false;
    QString mBackendState;
    PeerData mSelf;

    explicit Tailscale(QObject *parent = nullptr);

signals:
    void successChanged(bool);
    void versionChanged(const QString &);
    void isOperatorChanged(bool);
    void backendStateChanged(const QString &);
    void selfChanged(const PeerData &);
    void refreshed();

public:
    static Tailscale *instance();
    virtual ~Tailscale() = default;

    PeerModel *peerModel() const;
    ExitNodeModel *exitNodeModel() const;
    MullvadNodeModel *mullvadNodeModel() const;
    MullvadCountryModel *mullvadCountryModel() const;
    bool success() const;
    const QString &version() const;
    bool isOperator() const;
    const QString &backendState() const;
    const PeerData &self() const;

public slots:
    Q_INVOKABLE void up();
    Q_INVOKABLE void down();
    Q_INVOKABLE void toggle();
    Q_INVOKABLE void refresh();

    Q_INVOKABLE void setExitNode(const QString &ip);
    Q_INVOKABLE void unsetExitNode();
};

#endif /* KTAILCTL_TAILSCALE_TAILSCALE_H */
