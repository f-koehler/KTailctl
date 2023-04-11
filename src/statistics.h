#ifndef TAILCTL_STATISTICS_H
#define TAILCTL_STATISTICS_H

#include "speed_statistics.h"
#include "status.h"

#include <QMap>

class Statistics : public QObject
{
    Q_OBJECT

private:
    Status *mStatus;
    QMap<QString, SpeedStatistics *> mSpeedUp;
    QMap<QString, SpeedStatistics *> mSpeedDown;

public:
    Statistics(Status *status = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE SpeedStatistics *speedUp(const QString &id);
    Q_INVOKABLE SpeedStatistics *speedDown(const QString &id);

private slots:
    void statusRefreshed(const Status &status);

signals:
    void speedUpChanged();
    void speedDownChanged();
};

#endif /* TAILCTL_STATISTICS_H */
