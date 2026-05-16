#include "app_connector.hpp"

#include <QJsonValue>
#include <QString>
#include <QStringLiteral>

void AppConnectorPreferences::updateFromJson(QJsonObject &json)
{
    mAdvertise = json.take(QStringLiteral("advertise")).toBool();
}
