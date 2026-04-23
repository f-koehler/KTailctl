#include "tailnet_status.hpp"

void TailnetStatus::updateFromJson(QJsonObject &json)
{
    mName = json.take(QStringLiteral("Name")).toString();
    mMagicDnsSuffix = json.take(QStringLiteral("MagicDnsSuffix")).toString();
    mMagicDnsEnabled = json.take(QStringLiteral("MagicDnsEnabled")).toBool();
}
