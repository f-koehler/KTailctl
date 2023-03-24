#include "taildrop_helper.hpp"

#include <QCommandLineParser>
#include <QJsonDocument>
#include <QLocalSocket>
#include <QObject>

TaildropHelper::TaildropHelper(bool enabled, const QString &executable, const QString &directory, const QString &strategy, QObject *parent)
    : QObject(parent)
    , m_enabled(enabled)
    , m_executable(executable)
    , m_directory(directory)
    , m_strategy(strategy)
{
    m_server.listen("/tmp/tailctl_taildrop.sock");
    QObject::connect(&m_server, &QLocalServer::newConnection, this, &TaildropHelper::onNewConnection);

    if (m_enabled) {
        restartProcess();
    }
}

void TaildropHelper::restartProcess()
{
    stopProcess();
    m_process.setProgram(m_executable);
    m_process.setArguments({
        "file",
        "get",
        "-loop=true",
        "-wait=true",
        QStringLiteral("-conflict=") + m_strategy,
        m_directory,
    });
}

void TaildropHelper::stopProcess()
{
    if (m_process.state() != QProcess::NotRunning) {
        m_process.kill();
    }
}

void TaildropHelper::onNewConnection()
{
    while (m_server.hasPendingConnections()) {
        QLocalSocket *socket = m_server.nextPendingConnection();
        if (socket == nullptr) {
            continue;
        }
        if (!socket->waitForReadyRead(1000)) {
            continue;
        }

        const auto json = QJsonDocument::fromJson(socket->readAll()).object();
        m_enabled = json["enabled"].toBool();
        if (!m_enabled) {
            stopProcess();
        }

        bool needs_restart = false;
        if (auto executable = json["executable"].toString(); executable != m_executable) {
            m_executable = executable;
            needs_restart = true;
        }
        if (auto directory = json["directory"].toString(); directory != m_directory) {
            m_directory = directory;
            needs_restart = true;
        }
        if (auto strategy = json["strategy"].toString(); strategy != m_strategy) {
            m_strategy = strategy;
            needs_restart = true;
        }
        if (needs_restart || (m_process.state() == QProcess::NotRunning)) {
            restartProcess();
        }
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption optionExecutable(
        {
            QStringLiteral("executable"),
        },
        "Tailscale executable to use");
    parser.addOption(optionExecutable);

    QCommandLineOption optionEnabled(
        {
            QStringLiteral("enabled"),
        },
        "Enable taildrop",
        "enabled");
    parser.addOption(optionEnabled);

    QCommandLineOption optionDirectory(
        {
            QStringLiteral("directory"),
        },
        "Directory to save files to",
        "directory");
    parser.addOption(optionDirectory);

    QCommandLineOption optionStrategy(
        {
            QStringLiteral("strategy"),
        },
        "Strategy to use for saving files (rename, skip, overwrite)",
        "strategy");
    parser.addOption(optionStrategy);

    parser.process(app);

    TaildropHelper helper((bool)parser.value(optionEnabled).toInt(),
                          parser.value(optionExecutable),
                          parser.value(optionDirectory),
                          parser.value(optionStrategy));

    return app.exec();
}