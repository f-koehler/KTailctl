add_library(ktailctl_taildrop STATIC
    taildrop_receiver.cpp
    taildrop_sender.cpp
)
target_link_libraries(ktailctl_taildrop
    Qt6::Widgets
    KF6::CoreAddons
    ktailctl_config
    ktailctl_util
)