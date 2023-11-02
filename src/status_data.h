#ifndef KTAILCTL_STATUS_DATA_H
#define KTAILCTL_STATUS_DATA_H

#include "peer_data.h"

struct StatusData {
    QString version;
    bool isTun;
    QString backendState;
    PeerData self;
    QVector<PeerData> peers;
};

void from_json(const json &j, StatusData &s);

#endif /* KTAILCTL_STATUS_DATA_H */
