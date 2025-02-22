#include "exit_node_model.hpp"
#include "peer_model.hpp"

ExitNodeModel::ExitNodeModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool ExitNodeModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    return index.data(PeerModel::IsExitNodeRole).toBool() && !index.data(PeerModel::IsMullvadRole).toBool();
}