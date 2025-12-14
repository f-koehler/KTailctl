#ifndef KTAILCTL_TAILSCALE_NEW_HPP
#define KTAILCTL_TAILSCALE_NEW_HPP

#include "property_list_model.hpp"
#include "status/status.hpp"
#include <QObject>

class TailscaleNew : public QObject
{
public:
    using PeerModel = PropertyListModel<PeerStatus, PropertyListModelOwnership::External>;

    Q_OBJECT
    Q_PROPERTY(Status *status READ status BINDABLE bindableStatus)

private:
    QProperty<Status *> mStatus;

public:
    explicit TailscaleNew(QObject *parent = nullptr)
        : QObject(parent)
        , mStatus(new Status(this))
    {
        mStatus->refresh();
        qCritical() << "Number of peers" << mStatus->peerModel()->rowCount();
    }

    Status *status() const
    {
        return mStatus;
    }

    QBindable<Status *> bindableStatus()
    {
        return {&mStatus};
    }
};

#endif // KTAILCTL_TAILSCALE_HPP
