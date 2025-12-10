#ifndef KTAILCTL_TAILDROP_TARGET_STATUS_HPP
#define KTAILCTL_TAILDROP_TARGET_STATUS_HPP

namespace TaildropTargetStatus
{
Q_NAMESPACE
QML_ELEMENT

enum class TaildropTargetStatus {
    Unknown,
    Available,
    NoNetmapAvailable,
    IpnStateNotRunning,
    MissingCapability,
    Offline,
    NoPeerInfo,
    UnsupportedOs,
    NoPeerApi,
    OwnedByOtherUser
};

Q_ENUM_NS(TaildropTargetStatus)
}

#endif // KTAILCTL_TAILDROP_TARGET_STATUS_HPP
