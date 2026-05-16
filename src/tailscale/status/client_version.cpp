#include "client_version.hpp"
#include <qhashfunctions.h>
#include <qjsonobject.h>

void ClientVersion::updateFromJson(QJsonObject &json)
{
    mRunningLatest = json.take(QStringLiteral("RunningLatest")).toBool();
    mLatestVersion = json.take(QStringLiteral("Version")).toString();
    mUrgentSecurityUpdate = json.take(QStringLiteral("UrgentSecurityUpdate")).toBool();
    mNotify = json.take(QStringLiteral("Notify")).toBool();
    mNotifyUrl = json.take(QStringLiteral("NotifyUrl")).toString();
    mNotifyText = json.take(QStringLiteral("NotifyText")).toString();
}
