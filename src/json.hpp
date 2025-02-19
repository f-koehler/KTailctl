#ifndef KTAILCTL_DATA_JSON_HPP
#define KTAILCTL_DATA_JSON_HPP

#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void from_json(const json &j, QString &string);
void from_json(const json &j, QStringList &value);

#endif /* KTAILCTL_DATA_JSON_HPP */
