add_ktailctl_test(test_location
    SOURCES
        test_location.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/location.cpp)

add_ktailctl_test(test_user_profile
    SOURCES
        test_user_profile.cpp
        ${CMAKE_SOURCE_DIR}/src/tailscale/status/user_profile.cpp)
