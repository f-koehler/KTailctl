#include "auto_update.hpp"

#include <QJsonValue>
#include <QString>
#include <QStringLiteral>

void AutoUpdatePreferences::updateFromJson(QJsonObject &json)
{
    mCheck = json.take(QStringLiteral("Check")).toBool();
    mApply = json.take(QStringLiteral("Apply")).toBool();
}
