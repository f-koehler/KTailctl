if(KTAILCTL_FETCH_GO)
  include(FetchContent)

  # Parse required Go version from go.mod
  file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/go.mod" _go_mod_lines)
  foreach(_line IN LISTS _go_mod_lines)
    if(_line MATCHES "^go ([0-9]+\\.[0-9]+(\\.[0-9]+)?)")
      set(_go_version_required "${CMAKE_MATCH_1}")
      break()
    endif()
  endforeach()
  if(NOT DEFINED _go_version_required)
    message(FATAL_ERROR "KTAILCTL_FETCH_GO: could not parse Go version from go.mod")
  endif()
  message(STATUS "Required Go version (from go.mod): ${_go_version_required}")

  # Map CMake system name to Go OS identifier
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(_go_os "linux")
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(_go_os "darwin")
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(_go_os "windows")
  else()
    message(FATAL_ERROR "KTAILCTL_FETCH_GO: unsupported OS '${CMAKE_SYSTEM_NAME}'")
  endif()

  # Map CMake processor to Go architecture
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
    set(_go_arch "amd64")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)$")
    set(_go_arch "arm64")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^armv[67]")
    set(_go_arch "armv6l")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^i[3-6]86$")
    set(_go_arch "386")
  else()
    message(FATAL_ERROR "KTAILCTL_FETCH_GO: unsupported architecture '${CMAKE_SYSTEM_PROCESSOR}'")
  endif()

  set(_go_url "https://go.dev/dl/go${_go_version_required}.${_go_os}-${_go_arch}.tar.gz")
  message(STATUS "Fetching Go toolchain from: ${_go_url}")

  FetchContent_Declare(
    go_toolchain
    URL "${_go_url}"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  FetchContent_MakeAvailable(go_toolchain)

  set(GO_EXECUTABLE "${go_toolchain_SOURCE_DIR}/bin/go")
else()
  if(NOT DEFINED GO_EXECUTABLE)
    if(DEFINED $ENV{GO_EXECUTABLE})
      set(GO_EXECUTABLE $ENV{GO_EXECUTABLE})
    else()
      find_program(GO_EXECUTABLE go REQUIRED)
    endif()
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

add_library(ktailctl_wrapper_logging SHARED logging.cpp ${CMAKE_CURRENT_BINARY_DIR}/logging_tailscale_wrapper.cpp)
target_link_libraries(ktailctl_wrapper_logging PUBLIC Qt6::Core)
target_compile_definitions(ktailctl_wrapper_logging PRIVATE QT_MESSAGELOGCONTEXT)
target_include_directories(ktailctl_wrapper_logging PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

configure_file(logging.go.in ${CMAKE_CURRENT_SOURCE_DIR}/logging.go @ONLY)

if(NOT KTAILCTL_FLATPAK_BUILD)
  set(_go_toolchain_stamp "${CMAKE_CURRENT_BINARY_DIR}/go_toolchain.stamp")
  set(_go_vendor_stamp "${CMAKE_CURRENT_BINARY_DIR}/go_vendor.stamp")

  # Regenerated at CMake generate-time whenever GO_EXECUTABLE or its reported
  # version string changes. The new mtime triggers re-vendoring on the next build.
  file(GENERATE OUTPUT "${_go_toolchain_stamp}" CONTENT "${GO_EXECUTABLE}\n${GO_VERSION}")

  add_custom_command(
    OUTPUT "${_go_vendor_stamp}"
    COMMAND "${GO_EXECUTABLE}" mod vendor
    COMMAND "${CMAKE_COMMAND}" -E touch "${_go_vendor_stamp}"
    MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/go.mod"
    DEPENDS "${_go_toolchain_stamp}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Vendoring Go dependencies")

  set(_go_vendor_dep "${_go_vendor_stamp}")
else()
  # Flatpak: networkless build — vendor directory must already be present.
  set(_go_vendor_dep "${CMAKE_CURRENT_SOURCE_DIR}/go.sum")
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
            "${_go_vendor_dep}"
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
            "${_go_vendor_dep}"
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
