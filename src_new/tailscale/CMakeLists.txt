add_subdirectory(status)
add_subdirectory(wrapper)

target_sources(new_ktailctl PRIVATE tailscale.hpp)
