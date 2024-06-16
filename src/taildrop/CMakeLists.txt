add_library(ktailctl_taildrop STATIC
    # taildrop_receiver.cpp
    # taildrop_sender.cpp
    taildrop_send_job.cpp
    taildrop_receiver.cpp
)
target_link_libraries(ktailctl_taildrop
    Qt6::Widgets
    KF6::CoreAddons
    ktailctl_config
    ktailctl_util
    ktailctl_wrapper
)
