#include "json.hpp"

#include <algorithm>

void from_json(const json &j, QString &string)
{
    string = QString::fromStdString(j.get<std::string>());
}
void from_json(const json &j, QStringList &value)
{
    value.clear();
    for (const auto &item : j) {
        value.append(QString::fromStdString(item.get<std::string>()));
    }
}
