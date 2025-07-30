#include "account_data.hpp"
#include "json.hpp"
#include "logging_accounts_data.hpp"

void from_json(const nlohmann::json &j, AccountData &data)
{
    try {
        j.at("ControlURL").get_to<QString>(data.controlUrl);
        j.at("ID").get_to<QString>(data.id);
        j.at("Key").get_to<QString>(data.key);
        j.at("Name").get_to<QString>(data.name);
        j.at("NetworkProfile").at("DomainName").get_to<QString>(data.domainName);
        j.at("NetworkProfile").at("MagicDNSName").get_to<QString>(data.magicDnsName);
        j.at("UserProfile").at("DisplayName").get_to<QString>(data.userName);
        j.at("UserProfile").at("ID").get_to<quint64>(data.userId);
        j.at("UserProfile").at("LoginName").get_to<QString>(data.loginName);
    } catch (json::exception &e) {
        qCCritical(Logging::AccountsData) << "Error parsing accounts data: " << e.what();
    }
}
