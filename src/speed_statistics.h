#ifndef TAILCTL_SPEED_STATISTICS_H
#define TAILCTL_SPEED_STATISTICS_H

#include <QDateTime>
#include <QList>
#include <QObject>

class SpeedStatistics : public QObject
{
    Q_OBJECT

private:
    long mCapacity;
    QList<QDateTime> mTimestamps;
    QList<double> mValues;
    long mLastTransferred;
    QDateTime mLastTimestamp;

public:
    SpeedStatistics(QObject *parent = nullptr);

    Q_INVOKABLE double average(double window = 1.) const;

public slots:
    void update(long transferred);

signals:
    void averageChanged();
};

#endif /* TAILCTL_SPEED_STATISTICS_H */
