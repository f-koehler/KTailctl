#ifndef TAILCTL_SPEED_STATISTICS_H
#define TAILCTL_SPEED_STATISTICS_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <functional>

class SpeedStatistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double average1Second READ average NOTIFY refreshed)

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
    void refreshed();
};

#endif /* TAILCTL_SPEED_STATISTICS_H */
