#include "exit_node_model.h"

void ExitNodeModel::updatePeers(const Status &status)
{
    int i = 0;
    const auto size = mData.size();
    for (const auto &peer : status.statusData().peers) {
        if (!peer.isExitNode) {
            continue;
        }
        if (i < size) {
            if (mData[i].first == peer.hostName && mData[i].second == peer.dnsName) {
                ++i;
                continue;
            }
            mData[i].first = peer.hostName;
            mData[i].second = peer.dnsName;
            ++i;
            emit dataChanged(index(i), index(i));
            continue;
        }
        beginInsertRows(QModelIndex(), i, i);
        mData.append({peer.hostName, peer.dnsName});
        ++i;
        endInsertRows();
    }
    if (i < size) {
        beginRemoveRows(QModelIndex(), i, size - 1);
        mData.erase(mData.begin() + i, mData.end());
        endRemoveRows();
    }
}

ExitNodeModel::ExitNodeModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ExitNodeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mData.size();
}
QHash<int, QByteArray> ExitNodeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[HostNameRole] = "hostName";
    roles[DnsNameRole] = "dnsName";
    return roles;
}
QVariant ExitNodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QStringLiteral("Invalid index");
    }
    switch (role) {
    case HostNameRole:
        return mData.at(index.row()).first;
    case DnsNameRole:
        return mData.at(index.row()).second;
    default:
        return QStringLiteral("Unknown role");
    }
}
