#include "tailscale_helper.h"

#include <KAuth/HelperSupport>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QLocalSocket>

// void TailscaleHelper::restartTaildropProcess()
// {
//     stopTaildropProcess();

//     QProcess process;
//     process.setProgram(m_tailscale_executable);
//     process.setArguments({
//         "file",
//         "get",
//         "-loop=true",
//         "-wait=true",
//         QStringLiteral("-conflict=") + m_taildrop_strategy,
//         m_taildrop_directory,
//     });
//     qint64 pid;

//     if (!process.startDetached(&pid)) {
//         qDebug() << "Failed to start taildrop process";
//         return;
//     }
// }

// void TailscaleHelper::stopTaildropProcess()
// {
//     // if (m_taildrop_process.state() != QProcess::NotRunning) {
//     //     qDebug() << "Killing taildrop process";
//     //     m_taildrop_process.kill();
//     // }
// }
//
//

void TailscaleHelper::startTaildropHelper()
{
    QProcess process;
    process.setProgram("tailctl_taildrop_helper");
    process.setArguments({
        "--executable",
        m_tailscale_executable,
        "--enabled",
        m_taildrop_enabled ? "1" : "0",
        "--directory",
        m_taildrop_directory,
        "--strategy",
        m_taildrop_strategy,
    });
    process.startDetached();
}

void TailscaleHelper::updateTaildropStatus()
{
    const auto socket_path = QStringLiteral("/tmp/tailctl_taildrop.sock");
    QFile socket_file(socket_path);
    if (!socket_file.exists()) {
        startTaildropHelper();
        return;
    }

    QLocalSocket socket;
    socket.connectToServer("/tmp/tailctl_taildrop.sock");
    if (!socket.isWritable()) {
        startTaildropHelper();
        return;
    }
    const auto message = QString("{\"executable\": \"%1\", \"enabled\": %2, \"directory\": \"%3\", \"strategy\": \"%4\"}")
                             .arg(m_tailscale_executable)
                             .arg(m_taildrop_enabled)
                             .arg(m_taildrop_directory)
                             .arg(m_taildrop_strategy)
                             .toUtf8();
    socket.write(message);
}

KAuth::ActionReply TailscaleHelper::managetailscale(const QVariantMap &args)
{
    const auto operation = args["operation"].toString();
    if (operation == QStringLiteral("setTailscaleExecutable")) {
        return setTailscaleExecutable(args);
    }
    if (operation == QStringLiteral("setTaildropEnabled")) {
        return setTaildropEnabled(args);
    }
    if (operation == QStringLiteral("setTaildropDirectory")) {
        return setTaildropDirectory(args);
    }
    if (operation == QStringLiteral("setTaildropStrategy")) {
        return setTaildropStrategy(args);
    }
    return KAuth::ActionReply::HelperErrorReply();
}

KAuth::ActionReply TailscaleHelper::setTailscaleExecutable(const QVariantMap &args)
{
    const auto executable = args["executable"].toString();
    if (executable != m_tailscale_executable) {
        m_tailscale_executable = executable;
        if (m_taildrop_enabled) {
            updateTaildropStatus();
        }
    }
    return KAuth::ActionReply::SuccessReply();
}
KAuth::ActionReply TailscaleHelper::setTaildropEnabled(const QVariantMap &args)
{
    const auto enabled = args["enabled"].toBool();
    if (enabled != m_taildrop_enabled) {
        m_taildrop_enabled = enabled;
        updateTaildropStatus();
    }
    return KAuth::ActionReply::SuccessReply();
}
KAuth::ActionReply TailscaleHelper::setTaildropDirectory(const QVariantMap &args)
{
    const auto directory = args["directory"].toString();
    if (directory != m_taildrop_directory) {
        m_taildrop_directory = directory;
        if (m_taildrop_enabled) {
            updateTaildropStatus();
        }
    }
    return KAuth::ActionReply::SuccessReply();
}

KAuth::ActionReply TailscaleHelper::setTaildropStrategy(const QVariantMap &args)
{
    const auto strategy = args["strategy"].toString();
    if (strategy != m_taildrop_strategy) {
        m_taildrop_strategy = strategy;
        if (m_taildrop_enabled) {
            updateTaildropStatus();
        }
    }
    return KAuth::ActionReply::SuccessReply();
}

KAUTH_HELPER_MAIN("org.fkoehler.tailctl", TailscaleHelper)