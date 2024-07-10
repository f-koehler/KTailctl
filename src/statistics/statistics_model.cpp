#include "statistics_model.hpp"

StatisticsModel::StatisticsModel(const QString &nodeID, QObject *parent)
    : QAbstractListModel(parent)
    , mNodeID(nodeID)
{
}