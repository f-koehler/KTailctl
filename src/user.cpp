#include "user.h"

User::User(QObject *parent)
    : QObject(parent)
    , mData()
{
}

const quint64 &User::id() const
{
    return mData.id;
}
const QString &User::loginName() const
{
    return mData.loginName;
}
const QString &User::displayName() const
{
    return mData.displayName;
}
const QString &User::profilePicUrl() const
{
    return mData.profilePicUrl;
}

void User::update(UserData &newData)
{
    if (newData.id != mData.id) {
        mData.id = newData.id;
        emit idChanged(mData.id);
    }
    if (newData.loginName != mData.loginName) {
        mData.loginName.swap(newData.loginName);
        emit loginNameChanged(mData.loginName);
    }
    if (newData.displayName != mData.displayName) {
        mData.displayName.swap(newData.displayName);
        emit displayNameChanged(mData.displayName);
    }
    if (newData.profilePicUrl != mData.profilePicUrl) {
        mData.profilePicUrl.swap(newData.profilePicUrl);
        emit profilePicUrlChanged(mData.profilePicUrl);
    }
}