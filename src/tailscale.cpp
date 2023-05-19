// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tailscale.h"
#include "util.h"
#include <libtailctlpp.h>

#include <QProcess>

Tailscale::Tailscale(QObject *parent)
    : QObject(parent)
    , mStatus(new Status(this))
    , mStatistics(new Statistics(mStatus, this))
{
}

void Tailscale::toggle()
{
    if (mStatus->backendState() == "Running") {
        down();
    } else {
        up();
    }
}

Q_INVOKABLE void Tailscale::up()
{
    tailscale_up();
}

Q_INVOKABLE void Tailscale::down()
{
    tailscale_down();
}

Status *Tailscale::status()
{
    return mStatus;
}

Statistics *Tailscale::statistics()
{
    return mStatistics;
}