#ifndef KTAILCTL_KTAILCTL_NETWORKS_NETWORK_HPP
#define KTAILCTL_KTAILCTL_NETWORKS_NETWORK_HPP

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>

template<typename NodeType>
class Network : public QObject
{
    Q_OBJECT
public:
    using Node = NodeType;

    virtual const QHash<QString, QPointer<Node>> &getNodes() const = 0;
    virtual QHash<QString, QPointer<Node>> &getNodes() = 0;

signals:
    void nodesAdded(const QStringList &);
    void nodesRemoved(const QStringList &);
};

#endif /* KTAILCTL_KTAILCTL_NETWORKS_NETWORK_HPP */
