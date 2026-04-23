#include "app_connector.hpp"

void AppConnectorPreferences::updateFromJson(QJsonObject &json)
{
    mAdvertise = json.take(QStringLiteral("advertise")).toBool();
}
