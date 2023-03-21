#include "taildrop.h"

TaildropProcess::TaildropProcess(TailctlConfig *config, QObject *parent)
    : QObject(parent)
    , m_config(config)
    , m_tailscale_executable(config->tailscaleExecutable())
    , m_strategy(static_cast<TailctlConfig::EnumTaildropStrategy::type>(config->taildropStrategy()))
    , m_directory(config->taildropDirectory())
{
    if (config->taildropEnabled()) {
        restartProcess();
    }

    QObject::connect(config, &TailctlConfig::taildropEnabledChanged, this, &TaildropProcess::changeStatus);
    QObject::connect(config, &TailctlConfig::tailscaleExecutableChanged, this, &TaildropProcess::changeTailscaleExecutable);
    QObject::connect(config, &TailctlConfig::taildropStrategyChanged, this, &TaildropProcess::changeStrategy);
    QObject::connect(config, &TailctlConfig::taildropDirectoryChanged, this, &TaildropProcess::changeDirectory);
}

void TaildropProcess::restartProcess()
{
    qDebug() << "Restarting taildrop process";
    stopProcess();
    QString strategy;
    switch (m_strategy) {
    case TailctlConfig::EnumTaildropStrategy::Overwrite:
        strategy = "-conflict=overwrite";
        break;
    case TailctlConfig::EnumTaildropStrategy::Skip:
        strategy = "-conflict=skip";
        break;
    default:
        strategy = "-conflict=rename";
    }
    m_process.start(m_tailscale_executable, {"file", "get", "-loop=true", "-wait=true", strategy, m_directory});
}

void TaildropProcess::stopProcess()
{
    if (m_process.state() != QProcess::NotRunning) {
        m_process.kill();
    }
}

void TaildropProcess::changeStatus()
{
    if (!m_config->taildropEnabled()) {
        stopProcess();
    } else {
        if (m_process.state() == QProcess::NotRunning) {
            restartProcess();
        }
    }
}

void TaildropProcess::changeTailscaleExecutable()
{
    if (m_config->tailscaleExecutable() != m_tailscale_executable) {
        m_tailscale_executable = m_config->tailscaleExecutable();
        restartProcess();
    }
}

void TaildropProcess::changeStrategy()
{
    auto strategy = static_cast<TailctlConfig::EnumTaildropStrategy::type>(m_config->taildropStrategy());
    if (strategy != m_strategy) {
        m_strategy = strategy;
        restartProcess();
    }
}

void TaildropProcess::changeDirectory()
{
    if (m_config->taildropDirectory() != m_directory) {
        m_directory = m_config->taildropDirectory();
        restartProcess();
    }
}