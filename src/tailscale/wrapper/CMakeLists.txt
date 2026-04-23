if(NOT DEFINED GO_EXECUTABLE)
  if(DEFINED $ENV{GO_EXECUTABLE})
    set(GO_EXECUTABLE $ENV{GO_EXECUTABLE})
  else()
    find_program(GO_EXECUTABLE go REQUIRED)
  endif()
endif()
message(STATUS "Go executable: ${GO_EXECUTABLE}")
execute_process(COMMAND ${GO_EXECUTABLE} version OUTPUT_VARIABLE GO_VERSION)
message(STATUS "Go version: ${GO_VERSION}")

ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_tailscale_wrapper.hpp"
  IDENTIFIER
  "Logging::Tailscale::Wrapper"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Tailscale.Wrapper"
  DEFAULT_SEVERITY
  Info)

add_library(ktailctl_wrapper_logging SHARED logging.cpp)
target_link_libraries(ktailctl_wrapper_logging PUBLIC Qt6::Core)
target_compile_definitions(ktailctl_wrapper_logging PRIVATE QT_MESSAGELOGCONTEXT)
target_include_directories(ktailctl_wrapper_logging PRIVATE ${CMAKE_CURRENT_BINARY_DIR}})

configure_file(logging.go.in ${CMAKE_CURRENT_SOURCE_DIR}/logging.go @ONLY)

if(NOT KTAILCTL_FLATPAK_BUILD)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/go.sum
    COMMAND ${GO_EXECUTABLE} get .
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/go.mod
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Downloading Go dependencies")
endif()

set(KTAILCTL_WRAPPER_LIBRARY ${CMAKE_BINARY_DIR}/lib/libktailctl_wrapper.a)
set(KTAILCTL_WRAPPER_HEADER ${CMAKE_BINARY_DIR}/lib/libktailctl_wrapper.h)
message(STATUS "KTailctl wrapper library: ${KTAILCTL_WRAPPER_LIBRARY}")
message(STATUS "KTailctl wrapper header:  ${KTAILCTL_WRAPPER_HEADER}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_custom_command(
    OUTPUT ${KTAILCTL_WRAPPER_LIBRARY} ${KTAILCTL_WRAPPER_HEADER}
    COMMAND GOEXPERIMENT=cgocheck2 ${GO_EXECUTABLE} build -v -trimpath -mod vendor -o ${KTAILCTL_WRAPPER_LIBRARY}
            -buildmode=c-archive wrapper.go options.go taildrop.go callbacks.go logging.go
    MAIN_DEPENDENCY wrapper.go
    DEPENDS go.mod
            ${CMAKE_CURRENT_SOURCE_DIR}/go.sum
            ktailctl_wrapper_logging
            options.go
            taildrop.go
            callbacks.go
            logging.go
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building KTailctl wrapper library (Debug)")
else()
  add_custom_command(
    OUTPUT ${KTAILCTL_WRAPPER_LIBRARY} ${KTAILCTL_WRAPPER_HEADER}
    COMMAND ${GO_EXECUTABLE} build -v -trimpath -mod vendor -o ${KTAILCTL_WRAPPER_LIBRARY} -buildmode=c-archive
            wrapper.go options.go taildrop.go callbacks.go logging.go
    MAIN_DEPENDENCY wrapper.go
    DEPENDS go.mod
            ${CMAKE_CURRENT_SOURCE_DIR}/go.sum
            ktailctl_wrapper_logging
            options.go
            taildrop.go
            callbacks.go
            logging.go
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building KTailctl wrapper library")
endif()

add_custom_target(
  ktailctl_wrapper_target ALL
  DEPENDS ${KTAILCTL_WRAPPER_LIBRARY} ${KTAILCTL_WRAPPER_HEADER}
  COMMENT "Building KTailctl wrapper")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
add_library(ktailctl_wrapper SHARED IMPORTED GLOBAL)
set_target_properties(ktailctl_wrapper PROPERTIES IMPORTED_LOCATION ${KTAILCTL_WRAPPER_LIBRARY})
set_target_properties(ktailctl_wrapper PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/lib)
set_target_properties(ktailctl_wrapper PROPERTIES INTERFACE_LINK_DIRECTORIES ${CMAKE_BINARY_DIR}/lib)
set_target_properties(ktailctl_wrapper PROPERTIES INTERFACE_LINK_LIBRARIES_DIRECT ktailctl_wrapper_logging)
add_dependencies(ktailctl_wrapper ktailctl_wrapper_target)
add_library(KTailctl::Wrapper ALIAS ktailctl_wrapper)
add_library(KTailctl::WrapperLogging ALIAS ktailctl_wrapper_logging)

install(FILES ${KTAILCTL_WRAPPER_LIBRARY} TYPE LIB)
install(TARGETS ktailctl_wrapper_logging)

target_link_libraries(ktailctl PRIVATE KTailctl::Wrapper KTailctl::WrapperLogging)
target_sources(ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/logging_tailscale_wrapper.cpp)
