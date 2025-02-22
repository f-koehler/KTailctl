#include "mullvad_node_model.hpp"
#include "peer_model.hpp"

MullvadNodeModel::MullvadNodeModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MullvadNodeModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    return sourceModel()->index(source_row, 0, source_parent).data(PeerModel::IsMullvadRole).toBool();
}