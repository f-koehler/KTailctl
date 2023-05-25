#ifndef KTAILCTL_PREFERENCES_H
#define KTAILCTL_PREFERENCES_H

#include <QObject>

#include "libtailctlpp.h"

class Preferences : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool acceptRoutes READ acceptRoutes NOTIFY acceptRoutesChanged)

private:
    bool mAcceptRoutes;
    bool mAcceptDNS;
    QString mHostname;
    QString mOperatorUser;

signals:
    void acceptRoutesChanged(bool);
    void acceptDNSChanged(bool);
    void hostnameChanged(const QString &);
    void operatorUserChanged(const QString &);

public:
    Q_INVOKABLE void refresh();

    bool acceptRoutes() const;
    bool acceptDNS() const;
    const QString &hostname() const;
    const QString &operatorUser() const;
};

#endif /* KTAILCTL_PREFERENCES_H */
