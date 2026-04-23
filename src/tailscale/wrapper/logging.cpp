#include "logging_tailscale_wrapper.hpp"
#include <QLoggingCategory>
#include <QMessageLogContext>

extern "C" {

void ktailctl_critical(const char *message)
{
    qCCritical(Logging::Tailscale::Wrapper, "%s", message);
}
void ktailctl_debug(const char *message)
{
    qCDebug(Logging::Tailscale::Wrapper, "%s", message);
}
void ktailctl_info(const char *message)
{
    qCInfo(Logging::Tailscale::Wrapper, "%s", message);
}
void ktailctl_warning(const char *message)
{
    qCWarning(Logging::Tailscale::Wrapper, "%s", message);
}
}
