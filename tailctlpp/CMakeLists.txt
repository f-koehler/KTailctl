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

if(APPLE)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.dylib
            ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h
        COMMAND
            ${KTAILCTL_GO_EXECUTABLE} build -v -trimpath
                -o ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.dylib
                -buildmode=c-shared
                tailctlpp.go ${TAILCTLPP_DEPS}
        MAIN_DEPENDENCY tailctlpp.go
        DEPENDS go.mod go.sum ${TAILCTLPP_DEPS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_target(
        tailctlpp ALL DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.dylib
        ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h
    )
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/../bin/ktailctl.app/Contents/MacOS/libtailctlpp.dylib
        COMMAND
            cp ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.dylib
                ${CMAKE_CURRENT_BINARY_DIR}/../bin/ktailctl.app/Contents/MacOS/libtailctlpp.dylib
        MAIN_DEPENDENCY
            ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.dylib
    )
    add_custom_target(
        copy_tailctlpp ALL DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/../bin/ktailctl.app/Contents/MacOS/libtailctlpp.dylib
    )
else()
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.a
            ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h
        COMMAND
            ${KTAILCTL_GO_EXECUTABLE} build -v -trimpath
                -o ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.a
                -buildmode=c-archive
                tailctlpp.go ${TAILCTLPP_DEPS}
        MAIN_DEPENDENCY tailctlpp.go
        DEPENDS go.mod go.sum ${TAILCTLPP_DEPS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_target(
        tailctlpp ALL DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.a
        ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h
    )
endif()
