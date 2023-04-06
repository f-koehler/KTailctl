#include "notifier.h"

Notifier::Notifier(QObject *parent)
    : QObject(parent)
{
}

void Notifier::statusRefreshed(const Status &status)
{
    qDebug() << mLastBackendState << status.backendState();
    if (mLastBackendState.isEmpty()) {
        mLastBackendState = status.backendState();
        return;
    }

    if (mLastBackendState != status.backendState()) {
        mLastBackendState = status.backendState();
        if (status.backendState() == "Running") {
            auto notification = new KNotification("up");
            notification->setText("Tailscale is now up");
            notification->addContext("status", "up");
            notification->sendEvent();
        } else {
            auto notification = new KNotification("down");
            notification->setText("Tailscale is now down");
            notification->addContext("status", "down");
            notification->sendEvent();
        }
    }
}
