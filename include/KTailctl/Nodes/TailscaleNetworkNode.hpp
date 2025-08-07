#ifndef KTAILCTL_KTAILCTL_NODES_TAILSCALENETWORKNODE_HPP
#define KTAILCTL_KTAILCTL_NODES_TAILSCALENETWORKNODE_HPP

#include <KTailctl/Nodes/NetworkNode.hpp>
#include <KTailctl/Nodes/TailscaleNodeProperties.hpp>
#include <QObject>
#include <QPointer>
#include <QtQmlIntegration/qqmlintegration.h>

class TailscaleNetworkNode : public NetworkNode<TailscaleNodeProperties>
{
    Q_OBJECT
    QML_ELEMENT

private:
    QPointer<TailscaleNodeProperties> mProperties;

public:
    explicit TailscaleNetworkNode(const QString &id, QObject *parent = nullptr);
    explicit TailscaleNetworkNode(QString &&id, QObject *parent = nullptr);

    TailscaleNodeProperties *getProperties() override;
    const TailscaleNodeProperties *getProperties() const override;
};

#endif /* KTAILCTL_KTAILCTL_NODES_TAILSCALENETWORKNODE_HPP */
