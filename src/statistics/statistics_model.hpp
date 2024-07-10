#ifndef KTAILCTL_STATISTICS_STATISTICS_MODEL_HPP
#define KTAILCTL_STATISTICS_STATISTICS_MODEL_HPP

#include <QAbstractListModel>

class StatisticsModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<long> mRxBytes;
    QVector <

        public : explicit StatisticsModel(const QString &nodeID, QObject *parent = nullptr);
    virtual ~StatisticsModel() = default;
};

#endif /* KTAILCTL_STATISTICS_STATISTICS_MODEL_HPP */
