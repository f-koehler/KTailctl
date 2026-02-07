#ifndef KTAILCTL_SELF_HOSTED_EXIT_NODE_MODEL_HPP_
#define KTAILCTL_SELF_HOSTED_EXIT_NODE_MODEL_HPP_

#include "logging_tailscale_preferences.hpp"
#include "logging_tailscale_status.hpp"
#include "peer_status.hpp"
#include "property_list_model.hpp"

#include <QSortFilterProxyModel>
#include <qloggingcategory.h>

class SelfHostedExitNodeModel : public QSortFilterProxyModel
{
    Q_OBJECT

private:
    int mRoleIndexExitNode = -1;
    int mRoleIndexMullvadNode = -1;
    int mRoleIndexDnsName = -1;
    bool mRoleIndicesFound = false;

    using QSortFilterProxyModel::setSourceModel;

public:
    template<PropertyListModelOwnership Ownership>
    explicit SelfHostedExitNodeModel(PropertyListModel<PeerStatus, Ownership> *peerModel, QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
    {
        QSortFilterProxyModel::setSourceModel(reinterpret_cast<QAbstractItemModel *>(peerModel));

        mRoleIndicesFound = true;
        mRoleIndexExitNode = peerModel->roleIndexForProperty("exitNode");
        if (mRoleIndexExitNode == -1) [[unlikely]] {
            qCCritical(Logging::Tailscale::Status) << "Failed to find role index for exitNode";
            mRoleIndicesFound = false;
        }

        mRoleIndexMullvadNode = peerModel->roleIndexForProperty("mullvadNode");
        if (mRoleIndexMullvadNode == -1) [[unlikely]] {
            qCCritical(Logging::Tailscale::Status) << "Failed to find role index for mullvadNode";
            mRoleIndicesFound = false;
        }

        mRoleIndexDnsName = peerModel->roleIndexForProperty("dnsName");
        if (mRoleIndexDnsName == -1) [[unlikely]] {
            qCCritical(Logging::Tailscale::Status) << "Failed to find role index for dnsName";
            mRoleIndicesFound = false;
        }
    }

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        if (!mRoleIndicesFound) [[unlikely]] {
            return false;
        }

        const auto *model = sourceModel();
        const QModelIndex sourceIndex = model->index(sourceRow, 0, sourceParent);
        return !model->data(sourceIndex, mRoleIndexMullvadNode).toBool() && model->data(sourceIndex, mRoleIndexExitNode).toBool();
    }
};

#endif // KTAILCTL_MULLVAD_EXIT_NODE_MODEL_HPP
