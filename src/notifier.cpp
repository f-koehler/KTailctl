// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "notifier.h"

Notifier::Notifier(QObject *parent)
    : QObject(parent)
{
}

void Notifier::statusRefreshed(const Status &status)
{
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
