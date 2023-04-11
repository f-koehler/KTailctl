#include "tailscale.h"

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
    process.setProgram(mExecutable);
    process.setArguments({"up"});
    process.start();
    process.waitForFinished(3000);
}

Q_INVOKABLE void Tailscale::down()
{
    QProcess process;
    process.setProgram(mExecutable);
    process.setArguments({"down"});
    process.start();
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