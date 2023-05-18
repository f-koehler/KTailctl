// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tailscale.h"
#include "util.h"

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
    QProcess process;
    process.start("tailscale", {"up"});
    process.waitForFinished(3000);
}

Q_INVOKABLE void Tailscale::down()
{
    QProcess process;
    process.start("tailscale", {"down"});
    process.waitForFinished(3000);
}

Status *Tailscale::status()
{
    return mStatus;
}

Statistics *Tailscale::statistics()
{
    return mStatistics;
}