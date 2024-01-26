find_program(
    TAILWRAP_GO_EXECUTABLE
    go
    REQUIRED
)

add_library(tailwrap_logging_wrapper SHARED logging.cpp)
target_link_libraries(tailwrap_logging_wrapper PUBLIC Qt5::Core)
target_compile_definitions(tailwrap_logging_wrapper PRIVATE QT_MESSAGELOGCONTEXT)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/logging.go.in
    ${CMAKE_CURRENT_SOURCE_DIR}/logging.go
    @ONLY
)

if(NOT KTAILCTL_FLATPAK_BUILD)
    add_custom_command(
        OUTPUT go.sum
        COMMAND ${TAILWRAP_GO_EXECUTABLE} get .
        MAIN_DEPENDENCY go.mod
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set(TAILWRAP_LIBRARY ${CMAKE_BINARY_DIR}/lib/libtailwrap.so)
set(TAILWRAP_HEADER ${CMAKE_BINARY_DIR}/lib/libtailwrap.h)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(
        OUTPUT
            ${TAILWRAP_LIBRARY}
            ${TAILWRAP_HEADER}
        COMMAND
            GOEXPERIMENT=cgocheck2 ${TAILWRAP_GO_EXECUTABLE} build -v -trimpath
                -mod vendor
                -o ${TAILWRAP_LIBRARY}
                -buildmode=c-shared
                tailwrap.go options.go taildrop.go callbacks.go logging.go
        MAIN_DEPENDENCY tailwrap.go
        DEPENDS go.mod go.sum tailwrap_logging_wrapper options.go taildrop.go callbacks.go
                logging.go
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
else()
    add_custom_command(
        OUTPUT
            ${TAILWRAP_LIBRARY}
            ${TAILWRAP_HEADER}
        COMMAND
            ${TAILWRAP_GO_EXECUTABLE} build -v -trimpath
                -mod vendor
                -o ${TAILWRAP_LIBRARY}
                -buildmode=c-shared
                tailwrap.go options.go taildrop.go callbacks.go logging.go
        MAIN_DEPENDENCY tailwrap.go
        DEPENDS go.mod go.sum tailwrap_logging_wrapper options.go taildrop.go callbacks.go
                logging.go
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
add_custom_target(
    tailwrap_target ALL DEPENDS
    ${TAILWRAP_LIBRARY}
    ${TAILWRAP_HEADER}
)

install(FILES ${TAILWRAP_LIBRARY} TYPE LIB)
install(TARGETS tailwrap_logging_wrapper)