#ifndef KTAICTL_DATA_ACCOUNTS_DATA_HPP
#define KTAICTL_DATA_ACCOUNTS_DATA_HPP

#include <QString>
#include <nlohmann/json.hpp>

struct AccountData {
    QString controlUrl;
    QString id;
    QString key;
    QString name;
    QString domainName;
    QString magicDnsName;
    QString userName;
    quint64 userId;
    QString loginName;
};

void from_json(const nlohmann::json &j, AccountData &data);

#endif
