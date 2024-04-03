#ifndef KTAILCTL_STATUS_DATA_H
#define KTAILCTL_STATUS_DATA_H

#include "peer_data.h"
#include "user_data.h"

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logcat_status_data)

struct StatusData {
    QString version;
    bool isTun;
    QString backendState;
    PeerData self;
    QVector<PeerData> peers;
    QVector<UserData> users;
};

void from_json(const json &j, StatusData &s);

#endif /* KTAILCTL_STATUS_DATA_H */
