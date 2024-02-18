#include "exit_node_model.h"

ExitNode::ExitNode(const PeerData &data)
{
    name = data.hostName;
    if (!data.tailscaleIps.empty()) {
        ip = data.tailscaleIps.front();
    }
    if (data.location.has_value()) {
        countryCode = data.location.value().countryCode;
    }
    active = data.isCurrentExitNode;
    selfHosted = !data.isMullvad;
}

void ExitNodeModel::updatePeers(const Status &status)
{
    const auto old_size = mData.size();
    int i = 0;
    for (const auto &peer : status.statusData().peers) {
        if (!peer.isExitNode) {
            continue;
        }
        if (peer.tailscaleIps.empty()) {
            continue;
        }
        if (i < old_size) {
            const ExitNode exit_node(peer);
            if (exit_node == mData[i]) {
                ++i;
                continue;
            }
            mData[i] = exit_node;
            ++i;
            emit dataChanged(index(i), index(i));
            continue;
        }

        beginInsertRows(QModelIndex(), i, i);
        mData.append(ExitNode(peer));
        endInsertRows();
        ++i;
    }
    if (i < old_size) {
        beginRemoveRows(QModelIndex(), i, old_size - 1);
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
    roles[NameRole] = "name";
    roles[IpRole] = "ip";
    roles[CountryCodeRole] = "countryCode";
    roles[ActiveRole] = "active";
    roles[SelfHostedRole] = "selfHosted";
    roles[SectionRole] = "section";
    return roles;
}
QVariant ExitNodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QStringLiteral("Invalid index");
    }
    switch (role) {
    case NameRole:
        return mData.at(index.row()).name;
    case IpRole:
        return mData.at(index.row()).ip;
    case CountryCodeRole:
        return mData.at(index.row()).countryCode;
    case ActiveRole:
        return mData.at(index.row()).active;
    case SelfHostedRole:
        return mData.at(index.row()).selfHosted;
    case SectionRole:
        if (mData.at(index.row()).selfHosted) {
            return QStringLiteral("Self-Hosted");
        }
        return mData.at(index.row()).name.toUpper();
    default:
        return QStringLiteral("Unknown role");
    }
}
