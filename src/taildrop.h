#ifndef TAILCTL_TAILDROP_H
#define TAILCTL_TAILDROP_H

#include "tailctlconfig.h"
#include <QObject>
#include <QProcess>

class TaildropProcess : public QObject
{
    Q_OBJECT

private:
    TailctlConfig *m_config;
    QProcess m_process;
    QString m_tailscale_executable;
    TailctlConfig::EnumTaildropStrategy::type m_strategy;
    QString m_directory;

public slots:
    void changeStatus();
    void changeTailscaleExecutable();
    void changeStrategy();
    void changeDirectory();

public:
    TaildropProcess(TailctlConfig *config, QObject *parent = nullptr);

protected:
    void restartProcess();
    void stopProcess();
};

#endif /* TAILCTL_TAILDROP_H */
