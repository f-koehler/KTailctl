#ifndef KTAILCTL_TAILSCALE_MULLVAD_NODE_MODEL_HPP
#define KTAILCTL_TAILSCALE_MULLVAD_NODE_MODEL_HPP

#include <QSortFilterProxyModel>

class MullvadNodeModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

public:
    explicit MullvadNodeModel(QObject *parent = nullptr);
    virtual ~MullvadNodeModel() = default;
};

#endif /* KTAILCTL_TAILSCALE_MULLVAD_NODE_MODEL_HPP */
