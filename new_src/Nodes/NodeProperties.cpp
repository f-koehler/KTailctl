#include <KTailctl/Nodes/NodeProperties.hpp>

NodeProperties::NodeProperties(const QString &id, QObject *parent)
    : QObject(parent)
    , mId(id)
{
}
NodeProperties::NodeProperties(QString &&id, QObject *parent)
    : QObject(parent)
    , mId(id)
{
}

const QString &NodeProperties::getId() const
{
    return mId;
}
const QString &NodeProperties::getName() const
{
    return mName;
}
const QString &NodeProperties::getPublicKey() const
{
    return mPublicKey;
}
const QStringList &NodeProperties::getIpAddresses() const
{
    return mIpAddresses;
}