// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tailscale.h"
#include "util.h"

#include <QProcess>

Tailscale::Tailscale(QObject *parent)
    : QObject(parent)
    , mExecutable("tailscale")
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
    const auto [command, args] = composeTailscaleCommand(mExecutable, {"up"});
    process.start(command, args);
    process.waitForFinished(3000);
}

Q_INVOKABLE void Tailscale::down()
{
    QProcess process;
    const auto [command, args] = composeTailscaleCommand(mExecutable, {"down"});
    process.start(command, args);
    process.waitForFinished(3000);
}

const QString &Tailscale::executable() const
{
    return mExecutable;
}

Status *Tailscale::status()
{
    return mStatus;
}

Statistics *Tailscale::statistics()
{
    return mStatistics;
}

void Tailscale::setExecutable(const QString &executable)
{
    if (mExecutable == executable)
        return;

    mExecutable = executable;
    emit executableChanged();
}