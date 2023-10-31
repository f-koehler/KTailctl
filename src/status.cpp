#include "status.h"
#include "libtailwrap.h"

#include <algorithm>

Status::Status(QObject *parent)
    : QObject(parent)
{
}

bool Status::success() const
{
    return mSuccess;
}
bool Status::isOperator() const
{
    return mIsOperator;
}
const QString &Status::version() const
{
    return mData.version;
}
bool Status::isTun() const
{
    return mData.isTun;
}
const QString &Status::backendState() const
{
    return mData.backendState;
}
Peer *Status::self() const
{
    return mSelf;
}
const QVector<Peer *> &Status::peers() const
{
    return mPeers;
}

std::tuple<QList<Peer *>, QList<Peer *>> Status::exitNodes() const
{
    QList<Peer *> exit_nodes;
    QList<Peer *> mullvad_nodes;
    for (const Peer *peer : mPeers) {
        if (peer->isMullvad()) {
            mullvad_nodes.append(const_cast<Peer *>(peer));
        } else if (peer->isExitNode()) {
            exit_nodes.append(const_cast<Peer *>(peer));
        }
    }
    std::stable_sort(exit_nodes.begin(), exit_nodes.end(), [](const Peer *peer_a, const Peer *peer_b) {
        return peer_a->hostName() < peer_b->hostName();
    });
    std::stable_sort(mullvad_nodes.begin(), mullvad_nodes.end(), [](const Peer *peer_a, const Peer *peer_b) {
        if (peer_a->location() == nullptr || peer_b->location() == nullptr) {
            return peer_a->hostName() < peer_b->hostName();
        }
        if (peer_a->location()->countryCode() == peer_b->location()->countryCode()) {
            return peer_a->hostName() < peer_b->hostName();
        }
        return peer_a->location()->countryCode() < peer_b->location()->countryCode();
    });
    return {exit_nodes, mullvad_nodes};
}

void Status::update(StatusData &newData)
{
    if (newData.version != mData.version) {
        mData.version.swap(newData.version);
        emit versionChanged(mData.version);
    }
    if (newData.isTun != mData.isTun) {
        mData.isTun = newData.isTun;
        emit isTunChanged(mData.isTun);
    }
    if (newData.backendState != mData.backendState) {
        mData.backendState.swap(newData.backendState);
        emit backendStateChanged(mData.backendState);
    }
    if (mSelf == nullptr) {
        mSelf = new Peer(this);
    }
    mSelf->update(newData.self);

    // remove extra elements
    std::for_each(mPeers.begin() + newData.peers.size(), mPeers.end(), [](Peer *peer) {
        delete peer;
        peer = nullptr;
    });

    // add elements to match size of newData.peers
    mPeers.resize(newData.peers.size());

    // update elements
    for (int i = 0; i < newData.peers.size(); ++i) {
        if (mPeers[i] == nullptr) {
            mPeers[i] = new Peer(this);
        }
        mPeers[i]->update(newData.peers[i]);
    }
}
void Status::refresh()
{
    const char *jsonStr = tailscale_status();
    const bool success = jsonStr != nullptr;
    if (success) {
        json::parse(jsonStr).get_to<StatusData>(newData);
        update(newData);
        emit refreshed(*this);

        bool const newIsOperator = tailscale_is_operator() != 0U;
        if (newIsOperator != mIsOperator) {
            mIsOperator = newIsOperator;
            emit isOperatorChanged(mIsOperator);
        }
    }
    if (mSuccess != success) {
        mSuccess = success;
        emit successChanged(mSuccess);
    }
}
