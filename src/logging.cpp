#include "logging.h"
#include <fstream>
#include <iostream>
#include <sstream>

void handleLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    const QByteArray localMessage = message.toLocal8Bit();
    std::ostringstream strm;
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

    // std::ofstream logFile("/home/fkoehler/.cache/org.fkoehler.KTailctl.log");
}
