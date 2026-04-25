add_ktailctl_test(test_client_version
    SOURCES
        test_client_version.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/client_version.cpp)


add_ktailctl_test(test_exit_node_status
    SOURCES
        test_exit_node_status.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/exit_node_status.cpp)

add_ktailctl_test(test_location
    SOURCES
        test_location.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/location.cpp)

add_ktailctl_test(test_login_profile
    SOURCES
        test_login_profile.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/login_profile.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/user_profile.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/network_profile.cpp)

add_ktailctl_test(test_network_profile
    SOURCES
        test_network_profile.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/network_profile.cpp)

add_ktailctl_test(test_peer_status
    SOURCES
        test_peer_status.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/peer_status.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/location.cpp)

add_ktailctl_test(test_status_json
     SOURCES
         test_status_json.cpp
         test_status_wrapper_stub.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/status.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/peer_status.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/location.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/exit_node_status.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/tailnet_status.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/user_profile.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/client_version.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/mullvad_exit_node_model.cpp
         ${CMAKE_SOURCE_DIR}/src/tailscale/status/self_hosted_exit_node_model.cpp
         ${CMAKE_BINARY_DIR}/src/tailscale/status/logging_tailscale_status.cpp
         ${CMAKE_BINARY_DIR}/src/tailscale/preferences/logging_tailscale_preferences.cpp)

add_ktailctl_test(test_tailnet_status
    SOURCES
        test_tailnet_status.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/tailnet_status.cpp)

add_ktailctl_test(test_user_profile
    SOURCES
        test_user_profile.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/user_profile.cpp)
