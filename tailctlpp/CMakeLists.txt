find_program(
    KTAILCTL_GO_EXECUTABLE
    go
    REQUIRED
)

if(NOT KTAILCTL_FLATPAK_BUILD)
    add_custom_command(
        OUTPUT go.sum
        COMMAND ${KTAILCTL_GO_EXECUTABLE} get .
        MAIN_DEPENDENCY go.mod
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set(TAILCTLPP_DEPS callbacks.go options.go taildrop.go)

set(KTAILCTL_TAILCTLPP_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.so)
set(KTAILCTL_TAILCTLPP_HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h)

add_custom_command(
    OUTPUT
        ${KTAILCTL_TAILCTLPP_LIBRARY_PATH}
        ${KTAILCTL_TAILCTLPP_HEADER_PATH}
    COMMAND
        ${KTAILCTL_GO_EXECUTABLE} build -v -trimpath
            -o ${KTAILCTL_TAILCTLPP_LIBRARY_PATH}
            -buildmode=c-shared
            tailctlpp.go ${TAILCTLPP_DEPS}
    MAIN_DEPENDENCY tailctlpp.go
    DEPENDS go.mod go.sum ${TAILCTLPP_DEPS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
    tailctlpp ALL DEPENDS
    ${KTAILCTL_TAILCTLPP_LIBRARY_PATH}
    ${KTAILCTL_TAILCTLPP_HEADER_PATH}
)
