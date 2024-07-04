#ifndef KTAILCTL_TAILSCALE_PEER_MODEL_H
#define KTAILCTL_TAILSCALE_PEER_MODEL_H

#include "peer_data.h"
#include <QAbstractListModel>

class PeerModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<PeerData> mPeers;

    QList<int> updateRow(int row, const PeerData &peer);

public:
    enum Roles : int {
        TailscaleID = Qt::UserRole + 1,
        PublicKeyRole,
        HostNameRole,
        DnsNameRole,
        OsRole,
        TailscaleIpsRole,
        IsOnlineRole,
        IsActiveRole,
        IsExitNodeRole,
        IsCurrentExitNodeRole,
        IsRunningSSHRole,
        SSHCommandRole,
        TagsRole,
        IsMullvadRole,
        CountryRole,
        CountryCodeRole,
        CityRole,
        CityCodeRole,
    };

    explicit PeerModel(QObject *parent = nullptr);
    virtual ~PeerModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

    const QVector<PeerData> &peers() const;

public slots:
    void update(const QVector<PeerData> &peers);
};

#endif /* KTAILCTL_TAILSCALE_PEER_MODEL_H */
