#include "auto_update.hpp"
#include <qhashfunctions.h>
#include <qjsonobject.h>

void AutoUpdatePreferences::updateFromJson(QJsonObject &json)
{
    mCheck = json.take(QStringLiteral("Check")).toBool();
    mApply = json.take(QStringLiteral("Apply")).toBool();
}
