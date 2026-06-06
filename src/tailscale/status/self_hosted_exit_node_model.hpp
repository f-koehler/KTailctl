#ifndef KTAILCTL_SELF_HOSTED_EXIT_NODE_MODEL_HPP_
#define KTAILCTL_SELF_HOSTED_EXIT_NODE_MODEL_HPP_

#include <QAbstractItemModel>
#include <QLoggingCategory>
#include <QMessageLogger>
#include <QObject>
#include <QSortFilterProxyModel>
#include <Qt>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>

#include "logging_tailscale_status.hpp"
#include "peer_status.hpp"
#include "property_list_model.hpp"

class SelfHostedExitNodeModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ANONYMOUS

    int mRoleIndexExitNodeOption = -1;
    int mRoleIndexMullvadNode = -1;
    int mRoleIndexDnsName = -1;
    bool mRoleIndicesFound = false;

    using QSortFilterProxyModel::setSourceModel;

public:
    template<PropertyListModelOwnership Ownership>
    explicit SelfHostedExitNodeModel(PropertyListModel<PeerStatus, Ownership> *peerModel, QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
        , mRoleIndexExitNodeOption(peerModel->roleIndexForProperty("exitNodeOption"))
        , mRoleIndicesFound(true)
    {
        QSortFilterProxyModel::setSourceModel(static_cast<QAbstractItemModel *>(peerModel));

        if (mRoleIndexExitNodeOption == -1) [[unlikely]] {
            qCCritical(Logging::Tailscale::Status) << "Failed to find role index for exitNodeOption";
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

        sort(0, Qt::AscendingOrder);
    }

protected:
    [[nodiscard]] auto lessThan(const QModelIndex &left, const QModelIndex &right) const -> bool override;
    [[nodiscard]] auto filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const -> bool override;
};

#endif // KTAILCTL_MULLVAD_EXIT_NODE_MODEL_HPP
