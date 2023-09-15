add_library(tailwrap_logging_wrapper SHARED logging.cpp)
target_link_libraries(tailwrap_logging_wrapper PUBLIC Qt5::Core)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/logging.go.in
    ${CMAKE_CURRENT_SOURCE_DIR}/logging.go
    @ONLY
)

set(TAILWRAP_LOGGING_LIBRARY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libtailwrap_logging.so)

add_custom_command(
    OUTPUT
        ${TAILWRAP_LOGGING_LIBRARY}
    COMMAND
        ${TAILWRAP_GO_EXECUTABLE} build -v -trimpath
            -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libtailwrap_logging.so
            -buildmode=c-shared
            logging.go
    MAIN_DEPENDENCY logging.go
    DEPENDS go.mod tailwrap_logging_wrapper
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
    tailwrap_logging ALL DEPENDS
    ${TAILWRAP_LOGGING_LIBRARY}
)
