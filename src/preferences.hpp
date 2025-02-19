#ifndef KTAILCTL_CONFIG_PREFERENCES_HPP
#define KTAILCTL_CONFIG_PREFERENCES_HPP

#include <QObject>

#include "libktailctl_wrapper.h"

class Preferences : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool acceptRoutes READ acceptRoutes WRITE setAcceptRoutes NOTIFY acceptRoutesChanged)
    Q_PROPERTY(bool acceptDNS READ acceptDNS WRITE setAcceptDNS NOTIFY acceptDNSChanged)
    Q_PROPERTY(bool allowLANAccess READ allowLANAccess WRITE setExitNodeAllowLANAccess NOTIFY allowLANAccessChanged)
    Q_PROPERTY(bool advertiseExitNode READ advertiseExitNode WRITE setAdvertiseExitNode NOTIFY advertiseExitNodeChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(QString operatorUser READ operatorUser WRITE setOperatorUser NOTIFY operatorUserChanged)
    Q_PROPERTY(bool shieldsUp READ shieldsUp WRITE setShieldsUp NOTIFY shieldsUpChanged)
    Q_PROPERTY(bool ssh READ ssh WRITE setSSH NOTIFY sshChanged)
    Q_PROPERTY(QStringList trayIconThemes READ trayIconThemes CONSTANT)
    Q_PROPERTY(bool webClient READ webClient WRITE setWebClient NOTIFY webClientChanged)

private:
    bool mAcceptRoutes{};
    bool mAcceptDNS{};
    bool mAdvertiseExitNode{};
    QString mHostname;
    QString mOperatorUser;
    bool mShieldsUp{};
    bool mSSH{};
    bool mWebClient{};
    bool mExitNodeAllowLANAccess{};

    explicit Preferences(QObject *parent = nullptr);

signals:
    void acceptRoutesChanged(bool);
    void allowLANAccessChanged(bool);
    void acceptDNSChanged(bool);
    void advertiseExitNodeChanged(bool);
    void hostnameChanged(const QString &);
    void operatorUserChanged(const QString &);
    void shieldsUpChanged(bool);
    void sshChanged(bool);
    void webClientChanged(bool);

public:
    static Preferences *instance();
    virtual ~Preferences() = default;

    Q_INVOKABLE void refresh();

    bool acceptRoutes() const;
    bool acceptDNS() const;
    bool allowLANAccess() const;
    bool advertiseExitNode() const;
    const QString &hostname() const;
    const QString &operatorUser() const;
    bool shieldsUp() const;
    bool ssh() const;
    bool webClient() const;

    void setAcceptRoutes(bool acceptRoutes);
    void setExitNodeAllowLANAccess(bool allowLANAccess);
    void setAcceptDNS(bool acceptDNS);
    void setAdvertiseExitNode(bool advertiseExitNode);
    void setHostname(const QString &hostname);
    void setOperatorUser(const QString &operatorUser);
    void setShieldsUp(bool shieldsUp);
    void setSSH(bool ssh);
    void setWebClient(bool webClient);

    const QStringList &trayIconThemes();
};

#endif /* KTAILCTL_CONFIG_PREFERENCES_HPP */
