// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_SPEED_STATISTICS_H
#define KTAILCTL_SPEED_STATISTICS_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <functional>

class SpeedStatistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double average1Second READ average NOTIFY refreshed)
    Q_PROPERTY(QVariantList values READ valuesVariant NOTIFY refreshed)

private:
    long mCapacity;
    QList<QDateTime> mTimestamps;
    QList<double> mValues;
    long mLastTransferred;
    QDateTime mLastTimestamp;

public:
    SpeedStatistics(QObject *parent = nullptr);

    Q_INVOKABLE double average(double window = 1.) const;
    QVariantList valuesVariant() const;

public slots:
    void update(long transferred);

signals:
    void refreshed();
};

#endif /* KTAILCTL_SPEED_STATISTICS_H */
