find_program(
    KTAILCTL_GO_EXECUTABLE
    go
    REQUIRED
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.a ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h
    COMMAND go build -v -trimpath -o ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.a -buildmode=c-archive . 
    MAIN_DEPENDENCY tailctlpp.go
    DEPENDS go.mod go.sum
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(tailctlpp ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.a ${CMAKE_CURRENT_BINARY_DIR}/libtailctlpp.h)