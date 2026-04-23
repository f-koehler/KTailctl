#include "auto_update.hpp"

void AutoUpdatePreferences::updateFromJson(QJsonObject &json)
{
    mCheck = json.take(QStringLiteral("Check")).toBool();
    mApply = json.take(QStringLiteral("Apply")).toBool();
}
