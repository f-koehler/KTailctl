include(CTest)
find_package(
  Qt6 ${QT6_MIN_VERSION} REQUIRED
  COMPONENTS Test)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS GuiAddons)

set(KTAILCTL_LSAN_SUPPRESSIONS "${CMAKE_CURRENT_SOURCE_DIR}/lsan.supp")

function(add_ktailctl_test NAME)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;LIBS")
  add_executable(${NAME} ${ARG_SOURCES})
  target_link_libraries(${NAME} PRIVATE Qt6::Core Qt6::Test Qt6::Qml ${ARG_LIBS})
  target_include_directories(${NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}/src
        ${CMAKE_BINARY_DIR}/src/tailscale/status
        ${CMAKE_BINARY_DIR}/src/tailscale/preferences
        ${CMAKE_BINARY_DIR}/lib
    )
  add_dependencies(${NAME} ktailctl_wrapper_target)
  add_test(NAME ${NAME} COMMAND ${NAME})
  if(KTAILCTL_ENABLE_ASAN)
    set_property(TEST ${NAME} APPEND PROPERTY ENVIRONMENT
        "LSAN_OPTIONS=suppressions=${KTAILCTL_LSAN_SUPPRESSIONS}:print_suppressions=0")
  endif()
endfunction()

add_subdirectory(tailscale)
add_subdirectory(util)

add_ktailctl_test(test_property_list_model
    SOURCES
        test_property_list_model.cpp)
