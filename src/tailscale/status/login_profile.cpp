#include "login_profile.hpp"

#include <QJsonValue>
#include <QStringLiteral>
#include <utility>

void LoginProfile::updateFromJson(QJsonObject &json)
{
    mId = json.take(QStringLiteral("ID")).toString();
    mName = json.take(QStringLiteral("Name")).toString();
    mKey = json.take(QStringLiteral("Key")).toString();
    mNodeId = json.take(QStringLiteral("NodeID")).toString();
    mLocalUserId = json.take(QStringLiteral("LocalUserID")).toString();
    mControlUrl = json.take(QStringLiteral("ControlURL")).toString();

    auto networkJson = json.take(QStringLiteral("NetworkProfile")).toObject();
    mNetworkProfile->updateFromJson(networkJson);
    auto userProfileJson = json.take(QStringLiteral("UserProfile")).toObject();
    mUserProfile->updateFromJson(userProfileJson);
}
