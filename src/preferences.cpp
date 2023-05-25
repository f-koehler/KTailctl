#include "preferences.h"

Preferences::Preferences(QObject *parent)
    : QObject(parent)
{
    refresh();
}

void Preferences::refresh()
{
    GoUint8 tmpBool;
    GoString tmpString;

    if (tailscale_get_accept_routes(&tmpBool)) {
        if (static_cast<bool>(tmpBool) != mAcceptRoutes) {
            mAcceptRoutes = tmpBool;
            emit acceptRoutesChanged(mAcceptRoutes);
        }
    }

    if (tailscale_get_accept_dns(&tmpBool)) {
        if (static_cast<bool>(tmpBool) != mAcceptDNS) {
            mAcceptDNS = tmpBool;
            emit acceptDNSChanged(mAcceptDNS);
        }
    }

    if (tailscale_get_advertise_exit_node(&tmpBool)) {
        if (static_cast<bool>(tmpBool) != mAdvertiseExitNode) {
            mAdvertiseExitNode = tmpBool;
            emit advertiseExitNodeChanged(mAdvertiseExitNode);
        }
    }

    if (tailscale_get_hostname(&tmpString)) {
        QString tmp = QString::fromUtf8(tmpString.p, tmpString.n);
        if (tmp != mHostname) {
            mHostname = tmp;
            emit hostnameChanged(mHostname);
        }
    }

    if (tailscale_get_operator_user(&tmpString)) {
        QString tmp = QString::fromUtf8(tmpString.p, tmpString.n);
        if (tmp != mOperatorUser) {
            mOperatorUser = tmp;
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

void Preferences::setAcceptRoutes(bool acceptRoutes)
{
    GoUint8 tmp = static_cast<GoUint8>(acceptRoutes);
    if (tailscale_set_accept_routes(&tmp)) {
        mAcceptRoutes = acceptRoutes;
        emit acceptRoutesChanged(mAcceptRoutes);
    }
}
void Preferences::setAcceptDNS(bool acceptDNS)
{
    GoUint8 tmp = static_cast<GoUint8>(acceptDNS);
    if (tailscale_set_accept_dns(&tmp)) {
        mAcceptDNS = acceptDNS;
        emit acceptDNSChanged(mAcceptDNS);
    }
}
void Preferences::setAdvertiseExitNode(bool advertiseExitNode)
{
    GoUint8 tmp = static_cast<GoUint8>(advertiseExitNode);
    if (tailscale_set_advertise_exit_node(&tmp)) {
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
    if (tailscale_set_hostname(&tmp)) {
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
    if (tailscale_set_hostname(&tmp)) {
        mOperatorUser = user;
        emit operatorUserChanged(mOperatorUser);
    }
}