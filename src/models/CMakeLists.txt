add_library(ktailctl_models STATIC
    exit_node_model.cpp
    peer_model.cpp
)
target_link_libraries(ktailctl_models PUBLIC
    ktailctl_common
    ktailctl_data
)
