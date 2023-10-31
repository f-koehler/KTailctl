#ifndef KTAILCTL_STLTYPES_H
#define KTAILCTL_STLTYPES_H

#include <QDebug>
#define JSON_THROW_USER(exception)                                                                                                                             \
    {                                                                                                                                                          \
        qCritical() << "JSON error in " << __FILE__ << ":" << __LINE__ << " (function " << __FUNCTION__ << ") - " << (exception).what();                       \
        std::abort();                                                                                                                                          \
    }

#include <QString>
#include <QStringList>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void from_json(const json &j, QString &value);
void from_json(const json &j, QStringList &value);

#endif /* KTAILCTL_STLTYPES_H */
