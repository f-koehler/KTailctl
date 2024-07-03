#ifndef KTAILCTL_TAILSCALE_EXIT_NODE_MODEL_H
#define KTAILCTL_TAILSCALE_EXIT_NODE_MODEL_H

#include <QSortFilterProxyModel>

class ExitNodeModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

public:
    explicit ExitNodeModel(QObject *parent = nullptr);
    virtual ~ExitNodeModel() = default;
};

#endif /* KTAILCTL_TAILSCALE_EXIT_NODE_MODEL_H */
