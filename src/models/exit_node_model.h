#ifndef KTAILCTL_EXIT_NODE_MODEL_H
#define KTAILCTL_EXIT_NODE_MODEL_H

#include "status.h"

#include <QAbstractListModel>
#include <QPair>
#include <QVector>

class ExitNodeModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<QPair<QString, QString>> mData;

public slots:
    void updatePeers(const Status &status);

public:
    enum Roles : int {
        HostNameRole = Qt::UserRole + 1,
        IpRole,
    };

    explicit ExitNodeModel(QObject *parent = nullptr);
    virtual ~ExitNodeModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif /* KTAILCTL_EXIT_NODE_MODEL_H */
