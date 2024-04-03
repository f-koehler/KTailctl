#include "user_data.h"
#include <algorithm>

Q_LOGGING_CATEGORY(logcat_user_data, "org.fkoehler.KTailctl.UserData")

void from_json(const json &data, UserData &user)
{
    try {
        data.at("ID").get_to<quint64>(user.id);
        data.at("LoginName").get_to<QString>(user.loginName);
        data.at("DisplayName").get_to<QString>(user.displayName);
        data.at("ProfilePicUrl").get_to<QString>(user.profilePicUrl);
    } catch (json::exception &exception) {
        qCCritical(logcat_user_data) << "Error parsing user data: " << exception.what();
    }
}
void from_json(const json &data, QVector<UserData> &users)
{
    static constexpr quint64 taggedDevicesAccount = 38520981566058213UL;

    users.clear();
    users.reserve(static_cast<qsizetype>(data.size()));
    UserData user;
    for (const auto &entry : data.items()) {
        entry.value().get_to<UserData>(user);

        // skip "tagged-devices" account (used for Mullvad nodes)
        if (user.id == taggedDevicesAccount) {
            continue;
        }

        users.push_back(user);
    }
    std::stable_sort(users.begin(), users.end(), [](const UserData &a, const UserData &b) {
        return a.id < b.id;
    });
}