#ifndef KTAILCTL_KTAILCTL_NODES_NETWORKNODE_HPP
#define KTAILCTL_KTAILCTL_NODES_NETWORKNODE_HPP

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

template<typename PropertiesType>
class NetworkNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Properties *properties READ getProperties CONSTANT)
    QML_ELEMENT

public:
    using Properties = PropertiesType;

    explicit NetworkNode(QObject *parent = nullptr)
        : QObject(parent)
    {
    }
    virtual ~NetworkNode() = default;

    virtual Properties *getProperties() = 0;
    virtual const Properties *getProperties() const = 0;
};

#endif /* KTAILCTL_KTAILCTL_NODES_NETWORKNODE_HPP */
