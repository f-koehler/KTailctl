#include "tailnet_status.hpp"
#include <qhashfunctions.h>
#include <qjsonobject.h>

void TailnetStatus::updateFromJson(QJsonObject &json)
{
    mName = json.take(QStringLiteral("Name")).toString();
    mMagicDnsSuffix = json.take(QStringLiteral("MagicDnsSuffix")).toString();
    mMagicDnsEnabled = json.take(QStringLiteral("MagicDnsEnabled")).toBool();
}
