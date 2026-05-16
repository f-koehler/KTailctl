#include "exit_node_status.hpp"

#include <QJsonValue>
#include <QStringLiteral>
#include <QVariant>
#include <utility>

void ExitNodeStatus::updateFromJson(QJsonObject &json)
{
    mId = json.take(QStringLiteral("ID")).toString();
    mIsOnline = json.take(QStringLiteral("Online")).toBool();
    mTailscaleIps = json.take(QStringLiteral("TailscaleIPs")).toVariant().toStringList();
}
