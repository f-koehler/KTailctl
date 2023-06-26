find_program(
    KTAILCTL_GO_EXECUTABLE
    go
    REQUIRED
)

if(NOT KTAILCTL_FLATPAK_BUILD)
    add_custom_command(
        OUTPUT go.sum
        COMMAND go get .
        MAIN_DEPENDENCY go.mod
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

if(APPLE)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.dylib
            ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h
        COMMAND
            go build -v -trimpath
                -o ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.dylib
                -buildmode=c-shared
                tailctlpp.go callbacks.go
        MAIN_DEPENDENCY tailctlpp.go
        DEPENDS go.mod go.sum callbacks.go
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
            go build -v -trimpath
                -o ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.a
                -buildmode=c-archive
                tailctlpp.go callbacks.go
        MAIN_DEPENDENCY tailctlpp.go
        DEPENDS go.mod go.sum callbacks.go
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_target(
        tailctlpp ALL DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.a
        ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h
    )
endif()
