#include "preferences.hpp"
#include "themes.hpp"

Preferences::Preferences(QObject *parent)
    : QObject(parent)
{
    refresh();
}

Preferences *Preferences::instance()
{
    static Preferences instance;
    return &instance;
}

void Preferences::refresh()
{
    GoUint8 tmpBool = 0;

    if (tailscale_get_accept_routes(&tmpBool) != 0U) {
        if (static_cast<bool>(tmpBool) != mAcceptRoutes) {
            mAcceptRoutes = (tmpBool != 0U);
            emit acceptRoutesChanged(mAcceptRoutes);
        }
    }

    if (tailscale_get_exit_node_allow_lan_access(&tmpBool) != 0U) {
        if (static_cast<bool>(tmpBool) != mExitNodeAllowLANAccess) {
            mExitNodeAllowLANAccess = (tmpBool != 0U);
            emit allowLANAccessChanged(mExitNodeAllowLANAccess);
        }
    }

    if (tailscale_get_accept_dns(&tmpBool) != 0U) {
        if (static_cast<bool>(tmpBool) != mAcceptDNS) {
            mAcceptDNS = (tmpBool != 0U);
            emit acceptDNSChanged(mAcceptDNS);
        }
    }

    if (tailscale_get_advertise_exit_node(&tmpBool) != 0U) {
        if (static_cast<bool>(tmpBool) != mAdvertiseExitNode) {
            mAdvertiseExitNode = (tmpBool != 0U);
            emit advertiseExitNodeChanged(mAdvertiseExitNode);
        }
    }

    {
        std::unique_ptr<char, decltype(&free)> tmpString(tailscale_get_hostname(), free);
        if (tmpString != nullptr) {
            if (mHostname != tmpString.get()) {
                mHostname = tmpString.get();
                emit hostnameChanged(mHostname);
            }
        }
    }

    {
        std::unique_ptr<char, decltype(&free)> tmpString(tailscale_get_operator_user(), free);
        if (tmpString != nullptr) {
            if (mOperatorUser != tmpString.get()) {
                mOperatorUser = tmpString.get();
                emit operatorUserChanged(mOperatorUser);
            }
        }
    }

    if (tailscale_get_shields_up(&tmpBool) != 0U) {
        if (static_cast<bool>(tmpBool) != mShieldsUp) {
            mShieldsUp = (tmpBool != 0U);
            emit shieldsUpChanged(mShieldsUp);
        }
    }

    if (tailscale_get_ssh(&tmpBool) != 0U) {
        if (static_cast<bool>(tmpBool) != mSSH) {
            mSSH = (tmpBool != 0U);
            emit sshChanged(mSSH);
        }
    }

    if (tailscale_get_webclient(&tmpBool) != 0U) {
        if (static_cast<bool>(tmpBool) != mWebClient) {
            mWebClient = (tmpBool != 0U);
            emit webClientChanged(mWebClient);
        }
    }
}

bool Preferences::acceptRoutes() const
{
    return mAcceptRoutes;
}
bool Preferences::acceptDNS() const
{
    return mAcceptDNS;
}
bool Preferences::allowLANAccess() const
{
    return mExitNodeAllowLANAccess;
}
bool Preferences::advertiseExitNode() const
{
    return mAdvertiseExitNode;
}
const QString &Preferences::hostname() const
{
    return mHostname;
}
const QString &Preferences::operatorUser() const
{
    return mOperatorUser;
}
bool Preferences::shieldsUp() const
{
    return mShieldsUp;
}
bool Preferences::ssh() const
{
    return mSSH;
}
bool Preferences::webClient() const
{
    return mWebClient;
}

void Preferences::setAcceptRoutes(bool acceptRoutes)
{
    auto tmp = static_cast<GoUint8>(acceptRoutes);
    if (tailscale_set_accept_routes(&tmp) != 0U) {
        mAcceptRoutes = acceptRoutes;
        emit acceptRoutesChanged(mAcceptRoutes);
    }
}
void Preferences::setExitNodeAllowLANAccess(bool allowLANAccess)
{
    auto tmp = static_cast<GoUint8>(allowLANAccess);
    if (tailscale_set_exit_node_allow_lan_access(&tmp) != 0U) {
        mExitNodeAllowLANAccess = allowLANAccess;
        emit allowLANAccessChanged(mExitNodeAllowLANAccess);
    }
}
void Preferences::setAcceptDNS(bool acceptDNS)
{
    auto tmp = static_cast<GoUint8>(acceptDNS);
    if (tailscale_set_accept_dns(&tmp) != 0U) {
        mAcceptDNS = acceptDNS;
        emit acceptDNSChanged(mAcceptDNS);
    }
}
void Preferences::setAdvertiseExitNode(bool advertiseExitNode)
{
    auto tmp = static_cast<GoUint8>(advertiseExitNode);
    if (tailscale_set_advertise_exit_node(&tmp) != 0U) {
        mAdvertiseExitNode = advertiseExitNode;
        emit advertiseExitNodeChanged(mAdvertiseExitNode);
    }
}
void Preferences::setHostname(const QString &hostname)
{
    GoString tmp;
    QByteArray bytes = hostname.toUtf8();
    tmp.p = bytes.data();
    tmp.n = bytes.size();
    if (tailscale_set_hostname(&tmp) != 0U) {
        mHostname = hostname;
        emit hostnameChanged(mHostname);
    }
}
void Preferences::setOperatorUser(const QString &user)
{
    GoString tmp;
    QByteArray bytes = user.toUtf8();
    tmp.p = bytes.data();
    tmp.n = bytes.size();
    if (tailscale_set_hostname(&tmp) != 0U) {
        mOperatorUser = user;
        emit operatorUserChanged(mOperatorUser);
    }
}
void Preferences::setShieldsUp(bool shieldsUp)
{
    auto tmp = static_cast<GoUint8>(shieldsUp);
    if (tailscale_set_shields_up(&tmp) != 0U) {
        mShieldsUp = shieldsUp;
        emit shieldsUpChanged(mShieldsUp);
    }
}
void Preferences::setSSH(bool ssh)
{
    auto tmp = static_cast<GoUint8>(ssh);
    if (tailscale_set_ssh(&tmp) != 0U) {
        mSSH = ssh;
        emit sshChanged(mSSH);
    }
}
void Preferences::setWebClient(bool webClient)
{
    auto tmp = static_cast<GoUint8>(webClient);
    if (tailscale_set_webclient(&tmp) != 0U) {
        mWebClient = webClient;
        emit webClientChanged(mWebClient);
    }
}

const QStringList &Preferences::trayIconThemes()
{
    return TrayIconThemes;
}
