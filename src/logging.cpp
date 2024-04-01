#include "logging.h"
#include <QDateTime>
#include <QDir>
#include <fstream>
#include <iostream>
#include <sstream>

void handleLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    const QByteArray localMessage = message.toLocal8Bit();
    std::ostringstream strm;
    strm << '[' << QDateTime::currentDateTime().toString(Qt::ISODate).toStdString() << ']';
    switch (type) {
    case QtDebugMsg:
        strm << "[debug]";
        break;
    case QtInfoMsg:
        strm << "[info]";
        break;
    case QtWarningMsg:
        strm << "[warning]";
        break;
    case QtCriticalMsg:
        strm << "[critical]";
        break;
    case QtFatalMsg:
        strm << "[fatal]";
        break;
    }
    if (context.file != nullptr) {
        strm << '[' << context.file << ':' << context.line << ']';
    }
    strm << ' ' << localMessage.constData() << '\n';
    const auto formatted = strm.str();
    std::cerr << formatted;

    QString logDir = QDir::homePath() + "/.local/state";
    QByteArray const stateHomeArr = qgetenv("XDG_STATE_HOME");
    if (!stateHomeArr.isEmpty()) {
        logDir = QString::fromLocal8Bit(stateHomeArr);
    }
    std::string const logPath = logDir.toStdString() + "/org.fkoehler.KTailctl.log";
    std::ofstream logFile(logPath, std::ios_base::app);
    if (logFile.is_open()) {
        logFile << formatted;
        logFile.close();
    }
}
