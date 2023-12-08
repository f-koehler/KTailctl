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
    if (context.file) {
        strm << '[' << context.file << ':' << context.line << ']';
    }
    strm << ' ' << localMessage.constData() << '\n';
    const auto formatted = strm.str();
    std::cerr << formatted;

    std::cout << (QDir::homePath() + "/.cache/org.fkoehler.KTailctl.log").toStdString() << '\n';
    std::ofstream logFile((QDir::homePath() + "/.cache/org.fkoehler.KTailctl.log").toStdString(), std::ios_base::app);
    if (logFile.is_open()) {
        logFile << formatted;
        logFile.close();
    }
}
