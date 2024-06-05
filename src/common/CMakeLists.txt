add_library(ktailctl_common STATIC
    location.cpp
    peer.cpp
    status.cpp
)
target_link_libraries(ktailctl_common
    ktailctl_data
)
target_include_directories(
    ktailctl_common PUBLIC
    ${CMAKE_BINARY_DIR}/lib
)
target_link_directories(ktailctl_common PUBLIC ${CMAKE_BINARY_DIR}/lib)
