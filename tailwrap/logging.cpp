#include <QLoggingCategory>
#include <QMessageLogContext>

Q_LOGGING_CATEGORY(tailwrap, "org.fkoehler.KTailctl.tailwrap");

extern "C" {

void ktailctl_critical(const char *message)
{
    qCCritical(tailwrap, "%s", message);
}
void ktailctl_debug(const char *message)
{
    qCDebug(tailwrap, "%s", message);
}
void ktailctl_info(const char *message)
{
    qCInfo(tailwrap, "%s", message);
}
void ktailctl_warning(const char *message)
{
    qCWarning(tailwrap, "%s", message);
}
}
