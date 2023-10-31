#include "status_data.h"

void from_json(const json &j, StatusData &s)
{
    try {
        j.at("Version").get_to<QString>(s.version);
        j.at("TUN").get_to<bool>(s.isTun);
        j.at("BackendState").get_to<QString>(s.backendState);
        j.at("Self").get_to<PeerData>(s.self);
        // j.at("Peer").get_to<QVector<PeerData>>(s.peers);
    } catch (json::exception &e) {
        qCritical() << "Error parsing status data: " << e.what();
    }
}
