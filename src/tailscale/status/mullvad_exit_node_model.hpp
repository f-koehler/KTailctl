#ifndef KTAILCTL_MULLVAD_EXIT_NODE_MODEL_HPP
#define KTAILCTL_MULLVAD_EXIT_NODE_MODEL_HPP

#include "logging_tailscale_preferences.hpp"
#include "logging_tailscale_status.hpp"
#include "peer_status.hpp"
#include "property_list_model.hpp"
#include <QSortFilterProxyModel>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qloggingcategory.h>

class MullvadExitNodeModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ANONYMOUS

    int mRoleIndexMullvadNode = -1;
    int mRoleIndexDnsName = -1;
    bool mRoleIndicesFound = false;

    using QSortFilterProxyModel::setSourceModel;

public:
    template<PropertyListModelOwnership Ownership>
    explicit MullvadExitNodeModel(PropertyListModel<PeerStatus, Ownership> *peerModel, QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
        , mRoleIndexMullvadNode(peerModel->roleIndexForProperty("mullvadNode"))
        , mRoleIndicesFound(true)
    {
        QSortFilterProxyModel::setSourceModel(reinterpret_cast<QAbstractItemModel *>(peerModel));

        if (mRoleIndexMullvadNode == -1) [[unlikely]] {
            qCCritical(Logging::Tailscale::Status) << "Failed to find role index for mullvadNode";
            mRoleIndicesFound = false;
        }

        mRoleIndexDnsName = peerModel->roleIndexForProperty("dnsName");
        if (mRoleIndexDnsName == -1) [[unlikely]] {
            qCCritical(Logging::Tailscale::Status) << "Failed to find role index for dnsName";
            mRoleIndicesFound = false;
        }

        QSortFilterProxyModel::sort(0, Qt::AscendingOrder);
    }

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    [[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // KTAILCTL_MULLVAD_EXIT_NODE_MODEL_HPP
