#ifndef TAILCTL_SPEED_STATISTICS_H
#define TAILCTL_SPEED_STATISTICS_H

#include <QDateTime>
#include <QLineSeries>
#include <QList>
#include <QObject>
#include <functional>

class SpeedStatistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double average1Second READ average NOTIFY averageChanged)
    Q_PROPERTY(QtCharts::QLineSeries *series READ series CONSTANT)

private:
    long mCapacity;
    long mLast;
    long mLastMSecsSinceEpoch;
    QtCharts::QLineSeries *mSeries;

public:
    SpeedStatistics(QObject *parent = nullptr);

    Q_INVOKABLE qreal average(qreal windowMSecs = 1000.) const;
    QtCharts::QLineSeries *series();

public slots:
    void update(long transferred);

signals:
    void averageChanged();
};

#endif /* TAILCTL_SPEED_STATISTICS_H */
