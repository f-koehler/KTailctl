#include "network_profile.hpp"
#include <qhashfunctions.h>
#include <qjsonobject.h>

void NetworkProfile::updateFromJson(QJsonObject &json)
{
    mMagicDnsName = json.take(QStringLiteral("MagicDNSName")).toString();
    mDomainName = json.take(QStringLiteral("DomainName")).toString();
    mDisplayName = json.take(QStringLiteral("DisplayName")).toString();
}
