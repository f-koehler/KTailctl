#ifndef KTAILCTL_KTAILCTL_NETWORKS_TAILSCALENETWORK_HPP
#define KTAILCTL_KTAILCTL_NETWORKS_TAILSCALENETWORK_HPP

#include <KTailctl/Networks/Network.hpp>
#include <KTailctl/Nodes/TailscaleNetworkNode.hpp>
#include <QHash>
#include <QPointer>
#include <QString>

class TailscaleNetwork : public Network<TailscaleNetworkNode>
{
    Q_OBJECT

private:
    QHash<QString, QPointer<TailscaleNetworkNode>> mNodes;

public:
    const QHash<QString, QPointer<TailscaleNetworkNode>> &getNodes() const override;
    QHash<QString, QPointer<TailscaleNetworkNode>> &getNodes() override;
};

#endif /* KTAILCTL_KTAILCTL_NETWORKS_TAILSCALENETWORK_HPP */
