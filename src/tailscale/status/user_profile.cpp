#include "user_profile.hpp"

void UserProfile::updateFromJson(QJsonObject &json)
{
    mUserId = json.take(QStringLiteral("ID")).toInteger();
    mLoginName = json.take(QStringLiteral("LoginName")).toString();
    mDisplayName = json.take(QStringLiteral("DisplayName")).toString();
    mProfilePicUrl = json.take(QStringLiteral("ProfilePicURL")).toString();
}
