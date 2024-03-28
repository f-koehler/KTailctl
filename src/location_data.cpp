#include "location_data.h"

Q_LOGGING_CATEGORY(logcat_location_data, "org.fkoehler.KTailctl.LocationData")

void from_json(const json &j, LocationData &l)
{
    try {
        j.at("Country").get_to<QString>(l.country);
        j.at("CountryCode").get_to<QString>(l.countryCode);
        j.at("City").get_to<QString>(l.city);
        j.at("CityCode").get_to<QString>(l.cityCode);
        if (j.contains("Priority")) {
            j["Priority"].get_to<int>(l.priority);
        }
    } catch (json::exception &e) {
        qCCritical(logcat_location_data) << "Error parsing location data: " << e.what();
    }
}
