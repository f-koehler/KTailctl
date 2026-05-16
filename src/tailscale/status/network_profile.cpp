#include "network_profile.hpp"

#include <QJsonValue>
#include <QStringLiteral>
#include <utility>

void NetworkProfile::updateFromJson(QJsonObject &json)
{
    mMagicDnsName = json.take(QStringLiteral("MagicDNSName")).toString();
    mDomainName = json.take(QStringLiteral("DomainName")).toString();
    mDisplayName = json.take(QStringLiteral("DisplayName")).toString();
}
