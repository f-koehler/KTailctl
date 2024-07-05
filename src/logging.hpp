#ifndef KTAILCTL_LOGGING_HPP
#define KTAILCTL_LOGGING_HPP

#include <QFile>
#include <QMessageLogContext>
#include <QtGlobal>

void handleLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &message);

#endif /* KTAILCTL_LOGGING_HPP */
