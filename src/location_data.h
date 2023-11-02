#ifndef KTAILCTL_LOCATION_DATA_H
#define KTAILCTL_LOCATION_DATA_H

#include "json.h"
#include <QString>

struct LocationData {
    QString country;
    QString countryCode;
    QString city;
    QString cityCode;
    int priority;

    bool operator==(const LocationData &) const = default;
    bool operator!=(const LocationData &) const = default;
};

void from_json(const json &j, LocationData &l);

#endif /* KTAILCTL_LOCATION_DATA_H */
