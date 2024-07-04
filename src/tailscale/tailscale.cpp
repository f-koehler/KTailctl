#include "tailscale.h"
#include "libktailctl_wrapper.h"

Tailscale::Tailscale(QObject *parent)
    : QObject(parent)
    , mPeerModel(new PeerModel(this))
    , mExitNodeModel(new ExitNodeModel(this))
    , mMullvadNodeModel(new MullvadNodeModel(this))
    , mMullvadCountryModel(new MullvadCountryModel(this))
{
    mExitNodeModel->setSourceModel(mPeerModel);
    mMullvadNodeModel->setSourceModel(mPeerModel);
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
ExitNodeModel *Tailscale::exitNodeModel() const
{
    return mExitNodeModel;
}
MullvadNodeModel *Tailscale::mullvadNodeModel() const
{
    return mMullvadNodeModel;
}
MullvadCountryModel *Tailscale::mullvadCountryModel() const
{
    return mMullvadCountryModel;
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
const PeerData &Tailscale::self() const
{
    return mSelf;
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
        return a.mId < b.mId;
    });

    if (data.version != mVersion) {
        mVersion = data.version;
        emit versionChanged(mVersion);
    }
    if (data.backendState != mBackendState) {
        mBackendState = data.backendState;
        emit backendStateChanged(mBackendState);
    }
    if (data.self != mSelf) {
        mSelf = data.self;
        emit selfChanged(mSelf);
    }

    const bool isOperator = tailscale_is_operator();
    if (isOperator != mIsOperator) {
        mIsOperator = isOperator;
        emit isOperatorChanged(mIsOperator);
    }

    mPeerModel->update(data.peers);

    QMap<QString, QString> countries;
    // TODO: collect this data when bulding peer model?
    for (const PeerData &peer : data.peers) {
        if (peer.mIsMullvad) {
            countries.insert(peer.mCountryCode, peer.mCountry);
        }
    }
    mMullvadCountryModel->update(countries);

    // {
    //     const std::unique_ptr<char, decltype(&free)> suggestedExitNode(tailscale_suggest_exit_node(), free);
    //     const QString suggestedExitNodeStr = QString::fromUtf8(suggestedExitNode.get());
    //     if (suggestedExitNodeStr.isEmpty()) {
    //         if (mSuggestedExitNode != nullptr) {
    //             mSuggestedExitNode->deleteLater();
    //             mSuggestedExitNode = nullptr;
    //             emit suggestedExitNodeChanged(mSuggestedExitNode);
    //         }
    //     } else {
    //         for (Peer *peer : mPeers) {
    //             if (peer->id() == suggestedExitNodeStr) {
    //                 if (mSuggestedExitNode == nullptr) {
    //                     mSuggestedExitNode = new Peer(this);
    //                     emit suggestedExitNodeChanged(mSuggestedExitNode);
    //                 }
    //                 PeerData data = PeerData(peer->peerData());
    //                 mSuggestedExitNode->update(data);
    //                 break;
    //             }
    //         }
    //     }
    // }
    //

    emit refreshed();
}

void Tailscale::setExitNode(const QString &ip)
{
    const QByteArray bytes = ip.toUtf8();
    GoString tmp{bytes.data(), bytes.size()};
    tailscale_set_exit_node(&tmp);
}
void Tailscale::unsetExitNode()
{
    GoString tmp{nullptr, 0};
    tailscale_set_exit_node(&tmp);
}