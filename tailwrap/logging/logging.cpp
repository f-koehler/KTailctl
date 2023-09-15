#include <QLoggingCategory>
#include <QMessageLogContext>

Q_LOGGING_CATEGORY(ktailctl, "org.fkoehler.KTailctl.tailctlpp");

extern "C" {

void ktailctl_critical(const char *message)
{
    qCCritical(ktailctl, message);
}
void ktailctl_debug(const char *message)
{
    qCDebug(ktailctl, message);
}
void ktailctl_info(const char *message)
{
    qCInfo(ktailctl, message);
}
void ktailctl_warning(const char *message)
{
    qCWarning(ktailctl, message);
}
}
