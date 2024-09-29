#ifndef KTAILCTL_TAILSCALE_TAILSCALE_HPP
#define KTAILCTL_TAILSCALE_TAILSCALE_HPP

#include <QObject>

#include "exit_node_model.hpp"
#include "mullvad_country_model.hpp"
#include "mullvad_node_model.hpp"
#include "peer_model.hpp"
#include "status_data.hpp"

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
    Q_PROPERTY(bool hasSuggestedExitNode READ hasSuggestedExitNode NOTIFY hasSuggestedExitNodeChanged)
    Q_PROPERTY(bool hasCurrentExitNode READ hasCurrentExitNode NOTIFY hasCurrentExitNodeChanged)
    Q_PROPERTY(PeerData suggestedExitNode READ suggestedExitNode NOTIFY suggestedExitNodeChanged)
    Q_PROPERTY(PeerData currentExitNode READ currentExitNode NOTIFY currentExitNodeChanged)

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

    bool mHasSuggestedExitNode = false;
    bool mHasCurrentExitNode = false;
    PeerData mSuggestedExitNode;
    PeerData mCurrentExitNode;

    explicit Tailscale(QObject *parent = nullptr);

signals:
    void successChanged(bool);
    void versionChanged(const QString &);
    void isOperatorChanged(bool);
    void backendStateChanged(const QString &);
    void selfChanged(const PeerData &);
    void hasSuggestedExitNodeChanged(bool);
    void hasCurrentExitNodeChanged(bool);
    void suggestedExitNodeChanged(const PeerData &);
    void currentExitNodeChanged(const PeerData &);
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
    bool hasSuggestedExitNode() const;
    bool hasCurrentExitNode() const;
    const PeerData &suggestedExitNode() const;
    const PeerData &currentExitNode() const;

public slots:
    Q_INVOKABLE void up();
    Q_INVOKABLE void down();
    Q_INVOKABLE void toggle();
    Q_INVOKABLE void refresh();

    Q_INVOKABLE void setExitNode(const QString &dnsName);
    Q_INVOKABLE void unsetExitNode();
};

#endif /* KTAILCTL_TAILSCALE_TAILSCALE_HPP */
