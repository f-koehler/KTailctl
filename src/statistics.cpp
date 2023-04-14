#include "statistics.h"

#include <QFile>
#include <QTextStream>

Statistics::Statistics(Status *status, QObject *parent)
    : QObject(parent)
    , mStatus(status)
    , mTimerTotalSpeed(new QTimer(this))
    , mSpeedUpTotal(new SpeedStatistics(this))
    , mSpeedDownTotal(new SpeedStatistics(this))
{
    for (const auto &peer : mStatus->peers()) {
        auto iterUp = mSpeedUp.insert(peer->id(), new SpeedStatistics(this));
        auto iterDown = mSpeedDown.insert(peer->id(), new SpeedStatistics(this));
        iterUp.value()->update(peer->txBytes());
        iterDown.value()->update(peer->rxBytes());

        QObject::connect(peer, &Peer::txBytesChanged, iterUp.value(), &SpeedStatistics::update);
        QObject::connect(peer, &Peer::rxBytesChanged, iterDown.value(), &SpeedStatistics::update);
    }

    QObject::connect(mStatus, &Status::refreshed, this, &Statistics::statusRefreshed);

    QObject::connect(mTimerTotalSpeed, &QTimer::timeout, this, &Statistics::updateTotalUpSpeed);
    QObject::connect(mTimerTotalSpeed, &QTimer::timeout, this, &Statistics::updateTotalDownSpeed);
    mTimerTotalSpeed->setInterval(200);
    mTimerTotalSpeed->start();
}

SpeedStatistics *Statistics::speedUp(const QString &id)
{
    auto iter = mSpeedUp.find(id);
    if (iter == mSpeedUp.end()) {
        qCritical() << "No up speed statistics for peer" << id;
        return nullptr;
    }
    return iter.value();
}
SpeedStatistics *Statistics::speedDown(const QString &id)
{
    auto iter = mSpeedDown.find(id);
    if (iter == mSpeedDown.end()) {
        qCritical() << "No down speed statistics for peer" << id;
        return nullptr;
    }
    return iter.value();
}
SpeedStatistics *Statistics::speedUpTotal() const
{
    return mSpeedUpTotal;
}
SpeedStatistics *Statistics::speedDownTotal() const
{
    return mSpeedDownTotal;
}

void Statistics::statusRefreshed(const Status &status)
{
    bool newPeers = false;
    for (const auto &peer : status.peers()) {
        if (mSpeedUp.contains(peer->id())) {
            continue;
        }

        newPeers = true;

        auto iterUp = mSpeedUp.insert(peer->id(), new SpeedStatistics(this));
        auto iterDown = mSpeedDown.insert(peer->id(), new SpeedStatistics(this));
        iterUp.value()->update(peer->txBytes());
        iterDown.value()->update(peer->rxBytes());

        QObject::connect(peer, &Peer::txBytesChanged, iterUp.value(), &SpeedStatistics::update);
        QObject::connect(peer, &Peer::rxBytesChanged, iterDown.value(), &SpeedStatistics::update);
    }

    if (newPeers) {
        emit speedUpChanged();
        emit speedDownChanged();
    }
}

void Statistics::updateTotalUpSpeed()
{
    QFile file("/sys/class/net/tailscale0/statistics/tx_bytes");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical("Cannot read tx_bytes for tailscale0");
        return;
    }
    QTextStream stream(&file);
    long bytes;
    stream >> bytes;
    mSpeedUpTotal->update(bytes);
}

void Statistics::updateTotalDownSpeed()
{
    QFile file("/sys/class/net/tailscale0/statistics/tx_bytes");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical("Cannot read tx_bytes for tailscale0");
        return;
    }
    QTextStream stream(&file);
    long bytes;
    stream >> bytes;
    mSpeedUpTotal->update(bytes);
}