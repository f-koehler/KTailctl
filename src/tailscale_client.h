#ifndef TAILCTL_TAILSCALE_CLIENT_HPP
#define TAILCTL_TAILSCALE_CLIENT_HPP

#include "tailctlconfig.h"

#include <KAuth/Action>
#include <KAuth/ExecuteJob>
#include <QObject>

class TailscaleClient : public QObject
{
    Q_OBJECT

private:
    KAuth::Action m_action_manage_tailscale;

public:
    TailscaleClient(QObject *parent = nullptr);

public slots:
    void setTailscaleExecutable(const QString &executable);
    void setTaildropEnabled(bool enabled);
    void setTaildropDirectory(const QString &directory);
    void setTaildropStrategy(const TailctlConfig::EnumTaildropStrategy::type &strategy);
};

#endif /* TAILCTL_TAILSCALE_CLIENT_HPP */
