#include "speed_statistics.h"

#include <QDebug>

SpeedStatistics::SpeedStatistics(QObject *parent)
    : QObject(parent)
    , mCapacity(4096l)
{
    mLastTransferred = -1;
}

double SpeedStatistics::average(double window) const
{
    auto window_int = static_cast<qint64>(window * 1000.);

    if (mValues.size() == 0) {
        return 0.;
    }

    auto iter_values = mValues.crbegin();
    auto iter_timestamps = mTimestamps.crbegin();
    auto average = 0.0;
    auto samples = 0l;
    while (iter_values != mValues.crend()) {
        if (iter_timestamps->msecsTo(QDateTime::currentDateTime()) > window_int) {
            break;
        }
        average += *iter_values;
        ++samples;
        ++iter_values;
        ++iter_timestamps;
    }

    if (samples == 0) {
        return 0.;
    }

    return average / samples;
}

void SpeedStatistics::update(long transferred)
{
    auto now = QDateTime::currentDateTime();
    if (mLastTransferred >= 0) {
        auto dy = static_cast<double>(transferred - mLastTransferred);
        auto dx = static_cast<double>(mLastTimestamp.msecsTo(now)) / 1000.;
        mValues.append(dy / dx);
        mTimestamps.append(now);

        while (mValues.size() > mCapacity) {
            mValues.removeFirst();
            mTimestamps.removeFirst();
        }
    }

    emit refreshed();
    mLastTransferred = transferred;
    mLastTimestamp = now;
}