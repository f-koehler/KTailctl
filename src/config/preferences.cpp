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

    char *tmpString = tailscale_get_hostname();
    if (tmpString != nullptr) {
        const std::unique_ptr<char, decltype(&free)> tmpStringPtr(tmpString, free);
        if (mHostname != tmpString) {
            mHostname = tmpString;
            emit hostnameChanged(mHostname);
        }
    }

    tmpString = tailscale_get_operator_user();
    if (tmpString != nullptr) {
        const std::unique_ptr<char, decltype(&free)> tmpStringPtr(tmpString, free);
        if (mOperatorUser != tmpString) {
            mOperatorUser = tmpString;
            emit operatorUserChanged(mOperatorUser);
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

void Preferences::setAcceptRoutes(bool acceptRoutes)
{
    auto tmp = static_cast<GoUint8>(acceptRoutes);
    if (tailscale_set_accept_routes(&tmp) != 0U) {
        mAcceptRoutes = acceptRoutes;
        emit acceptRoutesChanged(mAcceptRoutes);
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

const QStringList &Preferences::trayIconThemes()
{
    return TrayIconThemes;
}
