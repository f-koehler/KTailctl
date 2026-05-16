#include "app_connector.hpp"
#include <qhashfunctions.h>
#include <qjsonobject.h>

void AppConnectorPreferences::updateFromJson(QJsonObject &json)
{
    mAdvertise = json.take(QStringLiteral("advertise")).toBool();
}
