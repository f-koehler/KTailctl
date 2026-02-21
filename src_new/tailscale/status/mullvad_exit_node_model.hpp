#ifndef KTAILCTL_MULLVAD_EXIT_NODE_MODEL_HPP
#define KTAILCTL_MULLVAD_EXIT_NODE_MODEL_HPP

#include "logging_tailscale_preferences.hpp"
#include "logging_tailscale_status.hpp"
#include "peer_status.hpp"
#include "property_list_model.hpp"

#include <QSortFilterProxyModel>
#include <qloggingcategory.h>

class MullvadExitNodeModel : public QSortFilterProxyModel
{
    Q_OBJECT

private:
    int mRoleIndexMullvadNode = -1;
    int mRoleIndexDnsName = -1;
    bool mRoleIndicesFound = false;

    using QSortFilterProxyModel::setSourceModel;

public:
    template<PropertyListModelOwnership Ownership>
    explicit MullvadExitNodeModel(PropertyListModel<PeerStatus, Ownership> *peerModel, QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
    {
        QSortFilterProxyModel::setSourceModel(reinterpret_cast<QAbstractItemModel *>(peerModel));

        mRoleIndicesFound = true;
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
        return model->data(sourceIndex, mRoleIndexMullvadNode).toBool();
    }
};

#endif // KTAILCTL_MULLVAD_EXIT_NODE_MODEL_HPP
