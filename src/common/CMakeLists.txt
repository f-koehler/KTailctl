add_library(ktailctl_common STATIC
    location.cpp
    peer.cpp
    status.cpp
)
target_link_libraries(ktailctl_common
    ktailctl_data
    ktailctl_wrapper
)
