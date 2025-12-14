#ifndef KTAILCTL_TAILSCALE_NEW_HPP
#define KTAILCTL_TAILSCALE_NEW_HPP

#include "property_list_model.hpp"
#include "status/status.hpp"
#include <QObject>

class TailscaleNew : public QObject
{
public:
    Q_OBJECT
    Q_PROPERTY(Status *status READ status CONSTANT)

private:
    Status *mStatus;

public:
    explicit TailscaleNew(QObject *parent = nullptr)
        : QObject(parent)
        , mStatus(new Status(this))
    {
        mStatus->refresh();
    }

    [[nodiscard]] Status *status() const noexcept
    {
        return mStatus;
    }
};

#endif // KTAILCTL_TAILSCALE_HPP
