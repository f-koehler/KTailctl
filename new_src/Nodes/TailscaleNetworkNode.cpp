#include <KTailctl/Nodes/TailscaleNetworkNode.hpp>

TailscaleNetworkNode::TailscaleNetworkNode(const QString &id, QObject *parent)
    : NetworkNode<TailscaleNodeProperties>(parent)
    , mProperties(new TailscaleNodeProperties(id, this))
{
}

TailscaleNetworkNode::TailscaleNetworkNode(QString &&id, QObject *parent)
    : NetworkNode<TailscaleNodeProperties>(parent)
    , mProperties(new TailscaleNodeProperties(id, this))
{
}

TailscaleNodeProperties *TailscaleNetworkNode::getProperties()
{
    return mProperties;
}
const TailscaleNodeProperties *TailscaleNetworkNode::getProperties() const
{
    return mProperties;
}