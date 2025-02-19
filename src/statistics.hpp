// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_STATISTICS_HPP
#define KTAILCTL_STATISTICS_HPP

#include "speed_statistics.hpp"

#include <QLoggingCategory>
#include <QMap>
#include <QTimer>

class Statistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SpeedStatistics *totalUpSpeed READ totalUpSpeed CONSTANT)
    Q_PROPERTY(SpeedStatistics *totalDownSpeed READ totalDownSpeed CONSTANT)

private:
    QMap<QString, SpeedStatistics *> mSpeedUp;
    QMap<QString, SpeedStatistics *> mSpeedDown;

    QTimer *mTimerTotalSpeed;
    SpeedStatistics *mSpeedUpTotal;
    SpeedStatistics *mSpeedDownTotal;

public:
    explicit Statistics(QObject *parent = nullptr);
    virtual ~Statistics() = default;

    Q_INVOKABLE SpeedStatistics *speedUp(const QString &id);
    Q_INVOKABLE SpeedStatistics *speedDown(const QString &id);
    SpeedStatistics *totalUpSpeed() const;
    SpeedStatistics *totalDownSpeed() const;

private slots:
    void statusRefreshed();
    void refreshTotalSpeed();

signals:
    void speedUpChanged();
    void speedDownChanged();
    void totalUpSpeedChanged();
    void totalDownSpeedChanged();
};

#endif /* KTAILCTL_STATISTICS_HPP */
