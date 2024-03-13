#ifndef KTAILCTL_LOGGING_H
#define KTAILCTL_LOGGING_H

#include <QFile>
#include <QMessageLogContext>
#include <QtGlobal>

void handleLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &message);

#endif /* KTAILCTL_LOGGING_H */
