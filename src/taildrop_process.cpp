#include "taildrop_process.h"
#include <QDebug>

TaildropProcess::TaildropProcess(const QString &executable, bool enabled, const QString &directory, const QString &strategy, QObject *parent)
    : QObject(parent)
    , m_executable(executable)
    , m_enabled(enabled)
    , m_directory(directory)
    , m_strategy(strategy)
{
    if (m_enabled) {
        restartProcess();
    }
}

void TaildropProcess::stopProcess()
{
    qDebug() << "Stopping taildrop process";
    if (m_process.state() != QProcess::NotRunning) {
        m_process.terminate();
        if (!m_process.waitForFinished(5000)) {
            m_process.kill();
            if (!m_process.waitForFinished(5000)) {
                qDebug() << "Failed to kill taildrop process:" << m_process.errorString();
            }
        }
    }
}

void TaildropProcess::restartProcess()
{
    stopProcess();
    if (m_enabled) {
        qDebug() << "Starting taildrop process";
        m_process.setProgram(m_executable);
        m_process.setArguments({
            "file",
            "get",
            "-loop=true",
            "-verbose=true",
            "-wait=true",
            "-conflict",
            m_strategy,
            m_directory,
        });
        m_process.start();
        if (!m_process.waitForStarted(5000)) {
            qDebug() << "Failed to start taildrop process:" << m_process.errorString();
        }
        if (m_process.state() != QProcess::Running) {
            qDebug() << "Failed to start taildrop process:" << m_process.errorString();
        }
    }
}

void TaildropProcess::setExecutable(const QString &executable)
{
    if (m_executable != executable) {
        m_executable = executable;
        restartProcess();
    }
}
void TaildropProcess::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        if (m_enabled) {
            restartProcess();
        } else {
            stopProcess();
        }
    }
}
void TaildropProcess::setDirectory(const QString &directory)
{
    if (m_directory != directory) {
        m_directory = directory;
        restartProcess();
    }
}
void TaildropProcess::setStrategy(const QString &strategy)
{
    if (m_strategy != strategy) {
        m_strategy = strategy;
        restartProcess();
    }
}