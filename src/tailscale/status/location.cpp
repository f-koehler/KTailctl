#include "location.hpp"
#include <qhashfunctions.h>
#include <qjsonobject.h>

void Location::updateFromJson(QJsonObject &json)
{
    mCountry = json.take(QStringLiteral("Country")).toString();
    mCountryCode = json.take(QStringLiteral("CountryCode")).toString();
    mCity = json.take(QStringLiteral("City")).toString();
    mCityCode = json.take(QStringLiteral("CityCode")).toString();
    mLatitude = json.take(QStringLiteral("Latitude")).toDouble();
    mLongitude = json.take(QStringLiteral("Longitude")).toDouble();
    mPriority = json.take(QStringLiteral("Priority")).toInt();
}
