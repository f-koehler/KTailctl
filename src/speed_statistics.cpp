#include "speed_statistics.h"

#include <QDebug>

SpeedStatistics::SpeedStatistics(QObject *parent)
    : QObject(parent)
    , mCapacity(4096l)
    , mLast(-1l)
    , mLastMSecsSinceEpoch(-1)
    , mSeries(new QtCharts::QLineSeries(this))
{
}

qreal SpeedStatistics::average(qreal windowMSecs) const
{
    if (mSeries->count() == 0) {
        return 0.;
    }

    qreal sum = 0.;
    int samples = 0;
    const auto now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    for (int i = mSeries->count() - 1; i >= 0; --i) {
        const auto point = mSeries->at(i);
        if (now - point.x() > windowMSecs) {
            break;
        }
        sum += point.y();
        ++samples;
    }

    if (samples == 0) {
        return 0.;
    }
    return sum / samples;
}
QtCharts::QLineSeries *SpeedStatistics::series()
{
    return mSeries;
}

void SpeedStatistics::update(long transferred)
{
    const auto now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (mLast >= 0) {
        const auto dy = static_cast<qreal>(transferred - mLast);
        const auto dx = now - mLastMSecsSinceEpoch;
        if (dx > 0) {
            mSeries->append(now, dy / (static_cast<qreal>(dx) / 1000.));

            if (mSeries->count() > mCapacity) {
                mSeries->remove(0, mSeries->count() - mCapacity);
            }
        }
    }

    emit averageChanged();
    mLast = transferred;
    mLastMSecsSinceEpoch = now;
}