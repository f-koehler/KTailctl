include(CTest)
find_package(
  Qt6 ${QT6_MIN_VERSION} REQUIRED
  COMPONENTS Test)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS GuiAddons)

function(add_ktailctl_test NAME)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;LIBS")
  add_executable(${NAME} ${ARG_SOURCES})
  target_link_libraries(${NAME} PRIVATE Qt6::Core Qt6::Test ${ARG_LIBS})
  target_include_directories(${NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}/src
        ${CMAKE_BINARY_DIR}/src/tailscale/status
        ${CMAKE_BINARY_DIR}/src/tailscale/preferences
        ${CMAKE_BINARY_DIR}/lib
    )
  add_test(NAME ${NAME} COMMAND ${NAME})
endfunction()

add_subdirectory(tailscale)
add_subdirectory(util)

add_ktailctl_test(test_property_list_model
    SOURCES
        test_property_list_model.cpp)
