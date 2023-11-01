#ifndef KTAILCTL_STLTYPES_H
#define KTAILCTL_STLTYPES_H

#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void from_json(const json &j, QString &value);
void from_json(const json &j, QStringList &value);

#endif /* KTAILCTL_STLTYPES_H */
