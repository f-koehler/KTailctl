#include "taildrop_process.h"
#include "util.h"

#include <QDebug>

TaildropProcess::TaildropProcess(const QString &executable, bool enabled, const QString &directory, const QString &strategy, QObject *parent)
    : QObject(parent)
    , mExecutable(executable)
    , mEnabled(enabled)
    , mDirectory(directory)
    , mStrategy(strategy)
{
    if (mEnabled) {
        restartProcess();
    }
}

void TaildropProcess::stopProcess()
{
    qDebug() << "Stopping taildrop process";
    if (mProcess.state() != QProcess::NotRunning) {
        mProcess.terminate();
        if (!mProcess.waitForFinished(5000)) {
            mProcess.kill();
            if (!mProcess.waitForFinished(5000)) {
                qDebug() << "Failed to kill taildrop process:" << mProcess.errorString();
            }
        }
    }
}

void TaildropProcess::restartProcess()
{
    stopProcess();
    if (mEnabled) {
        const auto [command, args] = composeTailscaleCommand(mExecutable,
                                                             {
                                                                 "file",
                                                                 "get",
                                                                 "-loop=true",
                                                                 "-verbose=true",
                                                                 "-wait=true",
                                                                 "-conflict",
                                                                 mStrategy,
                                                                 mDirectory,
                                                             });
        mProcess.setProgram(mExecutable);
        QStringList arguments{
            "file",
            "get",
            "-loop=true",
            "-verbose=true",
            "-wait=true",
            "-conflict",
            mStrategy,
            mDirectory,
        };
        qDebug() << "Starting taildrop process: " << mExecutable << arguments;
        mProcess.setArguments(arguments);
        mProcess.start();
        if (!mProcess.waitForStarted(5000)) {
            qDebug() << "Failed to start taildrop process:" << mProcess.errorString();
        }
        if (mProcess.state() != QProcess::Running) {
            qDebug() << "Failed to start taildrop process:" << mProcess.errorString();
        }
    }
}

void TaildropProcess::setExecutable(const QString &executable)
{
    if (mExecutable != executable) {
        mExecutable = executable;
        restartProcess();
    }
}
void TaildropProcess::setEnabled(bool enabled)
{
    if (mEnabled != enabled) {
        mEnabled = enabled;
        if (mEnabled) {
            restartProcess();
        } else {
            stopProcess();
        }
    }
}
void TaildropProcess::setDirectory(const QString &directory)
{
    if (mDirectory != directory) {
        mDirectory = directory;
        restartProcess();
    }
}
void TaildropProcess::setStrategy(const QString &strategy)
{
    if (mStrategy != strategy) {
        mStrategy = strategy;
        restartProcess();
    }
}