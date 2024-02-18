#ifndef KTAILCTL_EXIT_NODE_MODEL_H
#define KTAILCTL_EXIT_NODE_MODEL_H

#include "status.h"

#include <QAbstractListModel>
#include <QPair>
#include <QVector>

struct ExitNode {
    QString name;
    QString ip;
    QString countryCode;
    bool active;
    bool selfHosted;

    explicit ExitNode(const PeerData &data);
    bool operator==(const ExitNode &rhs) const = default;
    bool operator!=(const ExitNode &rhs) const = default;
};

class ExitNodeModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<ExitNode> mData;

public slots:
    void updatePeers(const Status &status);

public:
    enum Roles : int {
        NameRole = Qt::UserRole + 1,
        IpRole,
        CountryCodeRole,
        ActiveRole,
        SelfHostedRole,
        SectionRole,
    };

    explicit ExitNodeModel(QObject *parent = nullptr);
    virtual ~ExitNodeModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif /* KTAILCTL_EXIT_NODE_MODEL_H */
