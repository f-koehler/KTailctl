#include "self_hosted_exit_node_model.hpp"

#include <QModelIndex>
#include <QString>
#include <QVariant>
#include <compare>

auto SelfHostedExitNodeModel::lessThan(const QModelIndex &left, const QModelIndex &right) const -> bool
{
    return sourceModel()->data(left, mRoleIndexDnsName).toString() < sourceModel()->data(right, mRoleIndexDnsName).toString();
}

auto SelfHostedExitNodeModel::filterAcceptsRow(const int sourceRow, const QModelIndex &sourceParent) const -> bool
{
    if (!mRoleIndicesFound) [[unlikely]] {
        return false;
    }

    const auto *model = sourceModel();
    const QModelIndex sourceIndex = model->index(sourceRow, 0, sourceParent);
    return !model->data(sourceIndex, mRoleIndexMullvadNode).toBool() && model->data(sourceIndex, mRoleIndexExitNodeOption).toBool();
}
