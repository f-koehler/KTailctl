#ifndef TAILCTL_STATISTICS_H
#define TAILCTL_STATISTICS_H

#include "speed_statistics.h"
#include "status.h"

#include <QMap>
#include <QTimer>

class Statistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SpeedStatistics *speedUpTotal READ speedUpTotal CONSTANT)
    Q_PROPERTY(SpeedStatistics *speedDownTotal READ speedDownTotal CONSTANT)

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
    SpeedStatistics *speedUpTotal() const;
    SpeedStatistics *speedDownTotal() const;

private slots:
    void statusRefreshed(const Status &status);

public slots:
    void updateTotalUpSpeed();
    void updateTotalDownSpeed();

signals:
    void speedUpChanged();
    void speedDownChanged();
};

#endif /* TAILCTL_STATISTICS_H */
