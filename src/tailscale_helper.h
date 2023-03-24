#ifndef TAILCTL_TAILSCALE_HELPER_HPP
#define TAILCTL_TAILSCALE_HELPER_HPP

#include <KAuth/ActionReply>
#include <QObject>
#include <QProcess>
#include <QVariantMap>

class TailscaleHelper : public QObject
{
    Q_OBJECT

private:
    QString m_tailscale_executable;

    // QProcess m_taildrop_process;
    bool m_taildrop_enabled;
    QString m_taildrop_directory;
    QString m_taildrop_strategy;

protected:
    void startTaildropHelper();
    void updateTaildropStatus();
    // void restartTaildropProcess();
    // void stopTaildropProcess();

public Q_SLOTS:
    KAuth::ActionReply managetailscale(const QVariantMap &args);

public:
    KAuth::ActionReply setTailscaleExecutable(const QVariantMap &args);
    KAuth::ActionReply setTaildropEnabled(const QVariantMap &args);
    KAuth::ActionReply setTaildropDirectory(const QVariantMap &args);
    KAuth::ActionReply setTaildropStrategy(const QVariantMap &args);
};

#endif /* TAILCTL_TAILSCALE_HELPER_HPP */
