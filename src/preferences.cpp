#include "preferences.h"

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
const QString &Preferences::hostname() const
{
    return mHostname;
}
const QString &Preferences::operatorUser() const
{
    return mOperatorUser;
}