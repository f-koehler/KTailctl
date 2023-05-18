find_program(
    KTAILCTL_GO_EXECUTABLE
    go
    REQUIRED
)

add_custom_target(
    ktailctl-helper ALL
    go get .
    COMMAND go build  -v -trimpath -o ${CMAKE_CURRENT_BINARY_DIR}/ktailctl-helper
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ktailctl-helper
)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ktailctl-helper DESTINATION bin)