#include "tailnet_status.hpp"

#include <QJsonValue>
#include <QStringLiteral>
#include <utility>

void TailnetStatus::updateFromJson(QJsonObject &json)
{
    mName = json.take(QStringLiteral("Name")).toString();
    mMagicDnsSuffix = json.take(QStringLiteral("MagicDnsSuffix")).toString();
    mMagicDnsEnabled = json.take(QStringLiteral("MagicDnsEnabled")).toBool();
}
