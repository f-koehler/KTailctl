#include "mullvad_exit_node_model.hpp"

bool MullvadExitNodeModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return sourceModel()->data(left, mRoleIndexDnsName).toString() < sourceModel()->data(right, mRoleIndexDnsName).toString();
}

bool MullvadExitNodeModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!mRoleIndicesFound) [[unlikely]] {
        return false;
    }

    const auto *model = sourceModel();
    const QModelIndex sourceIndex = model->index(sourceRow, 0, sourceParent);
    return model->data(sourceIndex, mRoleIndexMullvadNode).toBool();
}
