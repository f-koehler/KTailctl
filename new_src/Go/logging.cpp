#include "logging.h"
#include "logging_go.hpp"
#include <QDebug>
#include <QLoggingCategory>
#include <QMessageLogContext>

extern "C" {

void ktailctl_critical(const char *message)
{
    qCCritical(Logging::Go, "%s", message);
}
void ktailctl_debug(const char *message)
{
    qCDebug(Logging::Go, "%s", message);
}
void ktailctl_info(const char *message)
{
    qCInfo(Logging::Go, "%s", message);
}
void ktailctl_warning(const char *message)
{
    qCWarning(Logging::Go, "%s", message);
}
}
