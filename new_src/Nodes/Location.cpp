#include <KTailctl/Nodes/Location.hpp>

Location::Location(QObject *parent)
    : QObject(parent)
    , mCountryCode(QStringLiteral("UNK"))
    , mCountryName(QStringLiteral("Unknown"))
    , mCityCode(QStringLiteral("UNK"))
    , mCityName(QStringLiteral("Unknown"))
    , mLongitude(0.0)
    , mLatitude(0.0)
{
}

const QString &Location::getCountryCode() const
{
    return mCountryCode;
}
const QString &Location::getCountryName() const
{
    return mCountryName;
}
const QString &Location::getCityCode() const
{
    return mCityCode;
}
const QString &Location::getCityName() const
{
    return mCityName;
}
double Location::getLongitude() const
{
    return mLongitude;
}
double Location::getLatitude() const
{
    return mLatitude;
}