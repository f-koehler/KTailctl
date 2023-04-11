#include "statistics.h"

Statistics::Statistics(Status *status, QObject *parent)
    : QObject(parent)
    , mStatus(status)
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