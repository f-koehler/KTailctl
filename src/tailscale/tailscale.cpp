#include "tailscale.h"
#include "libktailctl_wrapper.h"

Tailscale::Tailscale(QObject *parent)
    : QObject(parent)
    , mPeerModel(new PeerModel(&mPeers, this))
    , mExitNodeModel(new PeerModel(&mExitNodes, this))
    , mSelf(new Peer(this))
{
}

Tailscale *Tailscale::instance()
{
    static Tailscale instance;
    return &instance;
}

PeerModel *Tailscale::peerModel() const
{
    return mPeerModel;
}
PeerModel *Tailscale::exitNodeModel() const
{
    return mExitNodeModel;
}
bool Tailscale::success() const
{
    return mSuccess;
}
const QString &Tailscale::version() const
{
    return mVersion;
}
bool Tailscale::isOperator() const
{
    return mIsOperator;
}
const QString &Tailscale::backendState() const
{
    return mBackendState;
}
Peer *Tailscale::self() const
{
    return mSelf;
}
const QVector<Peer *> &Tailscale::peers() const
{
    return mPeers;
}
const QVector<Peer *> &Tailscale::exitNodes() const
{
    return mExitNodes;
}
Peer *Tailscale::exitNode() const
{
    return mExitNode;
}
Peer *Tailscale::suggestedExitNode() const
{
    return mSuggestedExitNode;
}

void Tailscale::up()
{
    tailscale_up();
}
void Tailscale::down()
{
    tailscale_down();
}
void Tailscale::toggle()
{
    if (mBackendState == "Running") {
        down();
    } else {
        up();
    }
}
void Tailscale::refresh()
{
    StatusData data;
    {
        std::unique_ptr<char, decltype(&free)> jsonStr(tailscale_status(), free);
        if (jsonStr == nullptr) {
            if (mSuccess) {
                mSuccess = false;
                emit successChanged(mSuccess);
            }
            return;
        }
    }
    if (!mSuccess) {
        mSuccess = true;
        emit successChanged(mSuccess);
    }
    json::parse(tailscale_status()).get_to(data);
    std::stable_sort(data.peers.begin(), data.peers.end(), [](const PeerData &a, const PeerData &b) {
        return a.id < b.id;
    });

    if (data.version != mVersion) {
        mVersion = data.version;
        emit versionChanged(mVersion);
    }
    if (data.backendState != mBackendState) {
        mBackendState = data.backendState;
        emit backendStateChanged(mBackendState);
    }
    mSelf->update(data.self);

    // shrink mPeers if necessary
    QVector<int> exitNodeIndices;
    bool didPeersChange = mPeers.size() != data.peers.size();
    if (!mPeers.empty() && (mPeers.size() > data.peers.size())) {
        mPeerModel->beginRemoveRows(QModelIndex(), data.peers.size(), mPeers.size() - 1);
        std::for_each(mPeers.begin() + data.peers.size(), mPeers.end(), [](Peer *peer) {
            peer->deleteLater();
        });
        mPeers.erase(mPeers.begin() + data.peers.size(), mPeers.end());
        mPeerModel->endRemoveRows();
    }

    // update peers in mPeers
    for (int i = 0; i < mPeers.size(); ++i) {
        if (mPeers[i]->update(data.peers[i])) {
            mPeerModel->dataChanged(mPeerModel->index(i), mPeerModel->index(i));
            didPeersChange = true;
        }
        if (data.peers[i].isExitNode) {
            exitNodeIndices.append(i);
        }
        if (data.peers[i].isCurrentExitNode) {
            if (mExitNode == nullptr) {
                mExitNode = new Peer(this);
                emit exitNodeChanged(mPeers[i]);
            }
        }
    }

    // grow mPeers if necessary
    if (mPeers.size() < data.peers.size()) {
        mPeerModel->beginInsertRows(QModelIndex(), mPeers.size(), data.peers.size() - 1);
        for (int i = mPeers.size(); i < data.peers.size(); ++i) {
            mPeers.append(new Peer(this));
            didPeersChange = didPeersChange || mPeers[i]->update(data.peers[i]);
            if (data.peers[i].isExitNode) {
                exitNodeIndices.append(i);
            }
            if (data.peers[i].isCurrentExitNode) {
                if (mExitNode == nullptr) {
                    mExitNode = new Peer(this);
                    emit exitNodeChanged(mPeers[i]);
                }
            }
        }
        mPeerModel->endInsertRows();
    }
    if (didPeersChange) {
        emit peersChanged(mPeers);
    }

    // Sort exit nod indices:
    // Mullvad exit nodes first
    // Then order is determined by dnsName
    std::stable_sort(exitNodeIndices.begin(), exitNodeIndices.end(), [&data](int a, int b) {
        const PeerData &peerA = data.peers[a];
        const PeerData &peerB = data.peers[b];
        if (peerA.isMullvad != peerB.isMullvad) {
            return peerA.isMullvad;
        }
        return peerA.dnsName < peerB.dnsName;
    });

    bool didExitNodesChange = (mExitNodes.size() != exitNodeIndices.size());

    // shrink mExitNodes if necessary
    if (!mExitNodes.empty() || (mExitNodes.size() > exitNodeIndices.size())) {
        mExitNodeModel->beginRemoveRows(QModelIndex(), exitNodeIndices.size(), mExitNodes.size() - 1);
        std::for_each(mExitNodes.begin() + exitNodeIndices.size(), mExitNodes.end(), [](Peer *peer) {
            peer->deleteLater();
        });
        mExitNodes.erase(mExitNodes.begin() + exitNodeIndices.size(), mExitNodes.end());
        mExitNodeModel->endRemoveRows();
    }

    // update peers in mExitNodes
    for (int i = 0; i < mExitNodes.size(); ++i) {
        if (mExitNodes[i]->update(data.peers[exitNodeIndices[i]])) {
            didExitNodesChange = true;
            mExitNodeModel->dataChanged(mExitNodeModel->index(i), mExitNodeModel->index(i));
        }
    }

    // grow mExitNodes if necessary
    if (mExitNodes.size() < exitNodeIndices.size()) {
        mExitNodeModel->beginInsertRows(QModelIndex(), mExitNodes.size(), exitNodeIndices.size() - 1);
        for (int i = mExitNodes.size(); i < exitNodeIndices.size(); ++i) {
            mExitNodes.append(new Peer(this));
            didExitNodesChange = didExitNodesChange || mExitNodes[i]->update(data.peers[exitNodeIndices[i]]);
        }
        mExitNodeModel->endInsertRows();
    }

    if (didExitNodesChange) {
        emit exitNodesChanged(mExitNodes);
    }

    if (const bool newIsOperator = tailscale_is_operator(); newIsOperator != mIsOperator) {
        mIsOperator = newIsOperator;
        emit isOperatorChanged(mIsOperator);
    }

    {
        const std::unique_ptr<char, decltype(&free)> suggestedExitNode(tailscale_suggest_exit_node(), free);
        const QString suggestedExitNodeStr = QString::fromUtf8(suggestedExitNode.get());
        if (suggestedExitNodeStr.isEmpty()) {
            if (mSuggestedExitNode != nullptr) {
                mSuggestedExitNode->deleteLater();
                mSuggestedExitNode = nullptr;
                emit suggestedExitNodeChanged(mSuggestedExitNode);
            }
        } else {
            for (Peer *peer : mPeers) {
                if (peer->id() == suggestedExitNodeStr) {
                    if (mSuggestedExitNode == nullptr) {
                        mSuggestedExitNode = new Peer(this);
                        emit suggestedExitNodeChanged(mSuggestedExitNode);
                    }
                    PeerData data = PeerData(peer->peerData());
                    mSuggestedExitNode->update(data);
                    break;
                }
            }
        }
    }
}

void Tailscale::setExitNode(Peer *node)
{
    if (node == nullptr) {
        unsetExitNode();
        return;
    }

    GoUint8 false_ = 0;
    tailscale_set_advertise_exit_node(&false_);

    QByteArray bytes = node->tailscaleIps().front().toUtf8();
    GoString tmp{bytes.data(), bytes.size()};
    tailscale_set_exit_node(&tmp);
}
void Tailscale::setExitNodeFromDnsName(const QString &dns)
{
    // TODO: get rid of this
    for (Peer *peer : mPeers) {
        if (peer->dnsName() == dns) {
            setExitNode(peer);
            return;
        }
    }
}
void Tailscale::unsetExitNode()
{
    GoString tmp{nullptr, 0};
    tailscale_set_exit_node(&tmp);
}