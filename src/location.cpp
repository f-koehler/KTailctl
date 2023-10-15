#include "location.h"

#include <QDebug>

bool Location::setCountry(const QString &country)
{
    if (mCountry == country) {
        return false;
    }
    mCountry = country;
    emit countryChanged(mCountry);
    return true;
}
bool Location::setCountryCode(const QString &countryCode)
{
    if (mCountryCode == countryCode) {
        return false;
    }
    mCountryCode = countryCode;
    emit countryCodeChanged(mCountryCode);
    return true;
}
bool Location::setCity(const QString &city)
{
    if (mCity == city) {
        return false;
    }
    mCity = city;
    emit cityChanged(mCity);
    return true;
}
bool Location::setCityCode(const QString &cityCode)
{
    if (mCityCode == cityCode) {
        return false;
    }
    mCityCode = cityCode;
    emit cityCodeChanged(mCityCode);
    return true;
}
bool Location::setPriority(int priority)
{
    if (mPriority == priority) {
        return false;
    }
    mPriority = priority;
    emit priorityChanged(mPriority);
    return true;
}

void Location::setCountryFromJSON(const QJsonObject &json)
{
    if (json.contains("Country") && json["Country"].isString()) {
        setCountry(json["Country"].toString());
    } else {
        qWarning() << "Cannot find string \"Country\"";
    }
}
void Location::setCountryCodeFromJSON(const QJsonObject &json)
{
    if (json.contains("CountryCode") && json["CountryCode"].isString()) {
        setCountry(json["CountryCode"].toString());
    } else {
        qWarning() << "Cannot find string \"CountryCode\"";
    }
}
void Location::setCityFromJSON(const QJsonObject &json)
{
    if (json.contains("City") && json["City"].isString()) {
        setCountry(json["City"].toString());
    } else {
        qWarning() << "Cannot find string \"City\"";
    }
}
void Location::setCityCodeFromJSON(const QJsonObject &json)
{
    if (json.contains("CityCode") && json["CityCode"].isString()) {
        setCountry(json["CityCode"].toString());
    } else {
        qWarning() << "Cannot find string \"CityCode\"";
    }
}
void Location::setPriorityFromJSON(const QJsonObject &json)
{
    if (json.contains("Priority") && json["Priority"].isDouble()) {
        setPriority(json["Priority"].toInt());
    } else {
        qWarning() << "Cannot find number \"Priority\"";
    }
}

Location::Location(QObject *parent)
    : QObject(parent)
{
}

Location *Location::fromJSON(const QJsonObject &json, QObject *parent)
{
    Location *location = new Location(parent);
    location->setCountryFromJSON(json);
    location->setCountryCodeFromJSON(json);
    location->setCityFromJSON(json);
    location->setCityCodeFromJSON(json);
    location->setPriorityFromJSON(json);
    return location;
}
bool Location::setTo(const Location *other)
{
    bool result = setCountry(other->country());
    result |= setCountryCode(other->countryCode());
    result |= setCity(other->city());
    result |= setCityCode(other->cityCode());
    result |= setPriority(other->priority());
    return result;
}

const QString &Location::country() const
{
    return mCountry;
}
const QString &Location::countryCode() const
{
    return mCountryCode;
}
const QString &Location::city() const
{
    return mCity;
}
const QString &Location::cityCode() const
{
    return mCityCode;
}
int Location::priority() const
{
    return mPriority;
}

Location &Location::operator=(const Location &other)
{
    if (this == &other) {
        return *this;
    }
    this->setTo(&other);
    return *this;
}
bool Location::operator==(const Location &other) const
{
    return this->country() == other.country() && this->countryCode() == other.countryCode() && this->city() == other.city()
        && this->cityCode() == other.cityCode() && this->priority() == other.priority();
}
bool Location::operator!=(const Location &other) const
{
    return !(*this == other);
}