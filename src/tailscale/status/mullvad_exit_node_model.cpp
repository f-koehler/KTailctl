#include "mullvad_exit_node_model.hpp"

#include <QModelIndex>
#include <QString>
#include <QVariant>
#include <compare>

auto MullvadExitNodeModel::lessThan(const QModelIndex &left, const QModelIndex &right) const -> bool
{
    return sourceModel()->data(left, mRoleIndexDnsName).toString() < sourceModel()->data(right, mRoleIndexDnsName).toString();
}

auto MullvadExitNodeModel::filterAcceptsRow(const int sourceRow, const QModelIndex &sourceParent) const -> bool
{
    if (!mRoleIndicesFound) [[unlikely]] {
        return false;
    }

    const auto *model = sourceModel();
    const QModelIndex sourceIndex = model->index(sourceRow, 0, sourceParent);
    return model->data(sourceIndex, mRoleIndexMullvadNode).toBool();
}
