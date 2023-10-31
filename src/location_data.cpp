#include "location_data.h"

void from_json(const json &j, LocationData &l)
{
    j.at("Country").get_to<QString>(l.country);
    j.at("CountryCode").get_to<QString>(l.countryCode);
    j.at("City").get_to<QString>(l.city);
    j.at("CityCode").get_to<QString>(l.cityCode);
    j.at("Priority").get_to<int>(l.priority);
}
