#ifndef KTAILCTL_TAILSCALE_HPP
#define KTAILCTL_TAILSCALE_HPP

#include "peer.hpp"
#include "property_list_model.hpp"
#include <QObject>
#

class Tailscale : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PropertyListModel<Peer> *peerModel READ peerModel BINDABLE bindablePeerModel)

private:
    QProperty<PropertyListModel<Peer> *> m_peer_model;

public:
    explicit Tailscale(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    [[nodiscard]] PropertyListModel<Peer> *peerModel()
    {
        return m_peer_model;
    }

    [[nodiscard]] QBindable<PropertyListModel<Peer> *> bindablePeerModel()
    {
        return {&m_peer_model};
    }
};

#endif // KTAILCTL_TAILSCALE_HPP
