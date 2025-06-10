#include "peer_model.hpp"
#include <algorithm>

PeerModel::PeerModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PeerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mPeers.size();
}

QHash<int, QByteArray> PeerModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{TailscaleID, "tailscaleID"},
                                              {PublicKeyRole, "publicKey"},
                                              {HostNameRole, "hostName"},
                                              {DnsNameRole, "dnsName"},
                                              {OsRole, "os"},
                                              {TailscaleIpsRole, "tailscaleIps"},
                                              {IsOnlineRole, "isOnline"},
                                              {IsActiveRole, "isActive"},
                                              {IsExitNodeRole, "isExitNode"},
                                              {IsCurrentExitNodeRole, "isCurrentExitNode"},
                                              {IsRunningSSHRole, "isRunningSSH"},
                                              {TagsRole, "tags"},
                                              {IsMullvadRole, "isMullvad"},
                                              {CountryRole, "country"},
                                              {CountryCodeRole, "countryCode"},
                                              {CityRole, "city"},
                                              {CityCodeRole, "cityCode"},
                                              {AdminPanelUrl, "adminPanelUrl"}};
    return roles;
}

QVariant PeerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QStringLiteral("Invalid index");
    }
    switch (role) {
    case TailscaleID:
        return mPeers.at(index.row()).mId;
    case PublicKeyRole:
        return mPeers.at(index.row()).mPublicKey;
    case HostNameRole:
        return mPeers.at(index.row()).mHostName;
    case DnsNameRole:
        return mPeers.at(index.row()).mDnsName;
    case OsRole:
        return mPeers.at(index.row()).mOs;
    case TailscaleIpsRole:
        return mPeers.at(index.row()).mTailscaleIps;
    case IsOnlineRole:
        return mPeers.at(index.row()).mIsOnline;
    case IsActiveRole:
        return mPeers.at(index.row()).mIsActive;
    case IsExitNodeRole:
        return mPeers.at(index.row()).mIsExitNode;
    case IsCurrentExitNodeRole:
        return mPeers.at(index.row()).mIsCurrentExitNode;
    case SSHHostKeysRole:
        return mPeers.at(index.row()).mSshHostKeys;
    case IsRunningSSHRole:
        return mPeers.at(index.row()).isRunningSSH();
    case SSHCommandRole:
        return mPeers.at(index.row()).sshCommand();
    case TagsRole:
        return mPeers.at(index.row()).mTags;
    case IsMullvadRole:
        return mPeers.at(index.row()).mIsMullvad;
    case CountryRole:
        return mPeers.at(index.row()).mCountry;
    case CountryCodeRole:
        return mPeers.at(index.row()).mCountryCode;
    case CityRole:
        return mPeers.at(index.row()).mCity;
    case CityCodeRole:
        return mPeers.at(index.row()).mCityCode;
    case AdminPanelUrl:
        return mPeers.at(index.row()).mAdminPanelUrl;
    default:
        return QStringLiteral("Invalid role");
    }
}

const QVector<PeerData> &PeerModel::peers() const
{
    return mPeers;
}

void PeerModel::update(const QVector<PeerData> &peers)
{
    if (mPeers.size() > peers.size()) {
        beginRemoveRows(QModelIndex(), peers.size(), mPeers.size() - 1);
        mPeers.erase(mPeers.begin() + peers.size(), mPeers.end());
        endRemoveRows();
    }

    for (int i = 0; i < mPeers.size(); ++i) {
        const QList<int> updatedRoles = updateRow(i, peers[i]);
        if (!updatedRoles.isEmpty()) {
            emit dataChanged(index(i), index(i), updatedRoles);
        }
    }
    if (mPeers.size() < peers.size()) {
        beginInsertRows(QModelIndex(), mPeers.size(), peers.size() - 1);
        for (int i = mPeers.size(); i < peers.size(); ++i) {
            mPeers.append(peers[i]);
        }
        endInsertRows();
    }
}
QList<int> PeerModel::updateRow(int row, const PeerData &peer)
{
    QList<int> result;
    PeerData &current = mPeers[row];
    if (current.mId != peer.mId) {
        current.mId = peer.mId;
        result.append(TailscaleID);
    }
    if (current.mPublicKey != peer.mPublicKey) {
        current.mPublicKey = peer.mPublicKey;
        result.append(PublicKeyRole);
    }
    if (current.mHostName != peer.mHostName) {
        current.mHostName = peer.mHostName;
        result.append(HostNameRole);
    }
    if (current.mDnsName != peer.mDnsName) {
        current.mDnsName = peer.mDnsName;
        result.append(DnsNameRole);
    }
    if (current.mOs != peer.mOs) {
        current.mOs = peer.mOs;
        result.append(OsRole);
    }
    if (current.mTailscaleIps != peer.mTailscaleIps) {
        current.mTailscaleIps = peer.mTailscaleIps;
        result.append(TailscaleIpsRole);
    }
    if (current.mIsOnline != peer.mIsOnline) {
        current.mIsOnline = peer.mIsOnline;
        result.append(IsOnlineRole);
    }
    if (current.mIsActive != peer.mIsActive) {
        current.mIsActive = peer.mIsActive;
        result.append(IsActiveRole);
    }
    if (current.mIsExitNode != peer.mIsExitNode) {
        current.mIsExitNode = peer.mIsExitNode;
        result.append(IsExitNodeRole);
    }
    if (current.mIsCurrentExitNode != peer.mIsCurrentExitNode) {
        current.mIsCurrentExitNode = peer.mIsCurrentExitNode;
        result.append(IsCurrentExitNodeRole);
    }
    if (current.mSshHostKeys != peer.mSshHostKeys) {
        current.mSshHostKeys = peer.mSshHostKeys;
        result.append(SSHHostKeysRole);
    }
    if (current.sshCommand() != peer.sshCommand()) {
        result.append(SSHCommandRole);
    }
    if (current.mTags != peer.mTags) {
        current.mTags = peer.mTags;
        result.append(TagsRole);
    }
    if (current.mTags != peer.mTags) {
        current.mTags = peer.mTags;
        result.append(TagsRole);
    }
    if (current.mIsMullvad != peer.mIsMullvad) {
        current.mIsMullvad = peer.mIsMullvad;
        result.append(IsMullvadRole);
    }
    if (current.mCountry != peer.mCountry) {
        current.mCountry = peer.mCountry;
        result.append(CountryRole);
    }
    if (current.mCountryCode != peer.mCountryCode) {
        current.mCountryCode = peer.mCountryCode;
        result.append(CountryCodeRole);
    }
    if (current.mCity != peer.mCity) {
        current.mCity = peer.mCity;
        result.append(CityRole);
    }
    if (current.mCityCode != peer.mCityCode) {
        current.mCityCode = peer.mCityCode;
        result.append(CityCodeRole);
    }
    return result;
}
