#include "exit_node_model.h"

void ExitNodeModel::updatePeers(const Status &status)
{
    int i = 1;
    const auto size = mData.size();
    for (const auto &peer : status.statusData().peers) {
        if (!peer.isExitNode) {
            continue;
        }
        if (i < size) {
            if (peer.tailscaleIps.empty()) {
                continue;
            }
            if (mData[i].first == peer.hostName && mData[i].second == peer.tailscaleIps.front()) {
                ++i;
                continue;
            }
            mData[i].first = peer.hostName;
            mData[i].second = peer.tailscaleIps.front();
            ++i;
            emit dataChanged(index(i), index(i));
            continue;
        }
        if (peer.tailscaleIps.empty()) {
            continue;
        }
        beginInsertRows(QModelIndex(), i, i);
        mData.append({peer.hostName, peer.tailscaleIps.front()});
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
    mData.append({QStringLiteral(""), QStringLiteral("")});
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
    roles[IpRole] = "ip";
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
    case IpRole:
        return mData.at(index.row()).second;
    default:
        return QStringLiteral("Unknown role");
    }
}
