#ifndef KTAILCTL_DATA_STATUS_DATA_HPP
#define KTAILCTL_DATA_STATUS_DATA_HPP

#include "peer_data.hpp"

#include <QLoggingCategory>

struct StatusData {
    QString version;
    bool isTun;
    QString backendState;
    PeerData self;
    QVector<PeerData> peers;
};

void from_json(const json &j, StatusData &s);

#endif /* KTAILCTL_DATA_STATUS_DATA_HPP */
