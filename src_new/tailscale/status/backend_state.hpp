#ifndef KTAILCTL_BACKEND_STATUS_HPP
#define KTAILCTL_BACKEND_STATUS_HPP

#include <QtQmlIntegration/qqmlintegration.h>

namespace BackendState
{
Q_NAMESPACE
QML_ELEMENT

// corresponds to: https://pkg.go.dev/tailscale.com/ipn/ipnstate#Status
enum class BackendState { NoState, NeedsLogin, NeedsMachineAuth, Stopped, Starting, Running };

Q_ENUM_NS(BackendState);
}

#endif // KTAILCTL_BACKEND_STATUS_HPP
