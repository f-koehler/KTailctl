#include <QLoggingCategory>
#include <QMessageLogContext>

Q_LOGGING_CATEGORY(ktailctl, "org.fkoehler.KTailctl.tailwrap");

extern "C" {

void ktailctl_critical(const char *message)
{
    qCCritical(ktailctl, "%s", message);
}
void ktailctl_debug(const char *message)
{
    qCDebug(ktailctl, "%s", message);
}
void ktailctl_info(const char *message)
{
    qCInfo(ktailctl, "%s", message);
}
void ktailctl_warning(const char *message)
{
    qCWarning(ktailctl, "%s", message);
}
}
