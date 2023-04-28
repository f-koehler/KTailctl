#ifndef TAILCTL_STATISTICS_H
#define TAILCTL_STATISTICS_H

#include "speed_statistics.h"
#include "status.h"

#include <QMap>
#include <QTimer>

class Statistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SpeedStatistics *totalUpSpeed READ totalUpSpeed CONSTANT)
    Q_PROPERTY(SpeedStatistics *totalDownSpeed READ totalDownSpeed CONSTANT)

private:
    Status *mStatus;
    QMap<QString, SpeedStatistics *> mSpeedUp;
    QMap<QString, SpeedStatistics *> mSpeedDown;

    QTimer *mTimerTotalSpeed;
    SpeedStatistics *mSpeedUpTotal;
    SpeedStatistics *mSpeedDownTotal;

public:
    Statistics(Status *status = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE SpeedStatistics *speedUp(const QString &id);
    Q_INVOKABLE SpeedStatistics *speedDown(const QString &id);
    SpeedStatistics *totalUpSpeed() const;
    SpeedStatistics *totalDownSpeed() const;

private slots:
    void statusRefreshed(const Status &status);
    void refreshTotalSpeed();

signals:
    void speedUpChanged();
    void speedDownChanged();
    void totalUpSpeedChanged();
    void totalDownSpeedChanged();
};

#endif /* TAILCTL_STATISTICS_H */
