#ifndef KTAILCTL_ACCOUNT_DATA_H
#define KTAILCTL_ACCOUNT_DATA_H

#include "json.h"
#include <QLoggingCategory>
#include <QString>
#include <QVector>

Q_DECLARE_LOGGING_CATEGORY(logcat_account_data)

struct UserData {
    quint64 id;
    QString loginName;
    QString displayName;
    QString profilePicUrl;

    bool operator==(const UserData &) const = default;
    bool operator!=(const UserData &) const = default;
};

void from_json(const json &data, UserData &user);
void from_json(const json &data, QVector<UserData> &users);

#endif /* KTAILCTL_ACCOUNT_DATA_H */
