if(NOT DEFINED KTAILCTL_WRAPPER_GO_EXECUTABLE)
  if(DEFINED $ENV{KTAILCTL_WRAPPER_GO_EXECUTABLE})
    set(KTAILCTL_WRAPPER_GO_EXECUTABLE $ENV{KTAILCTL_WRAPPER_GO_EXECUTABLE})
  else()
    find_program(KTAILCTL_WRAPPER_GO_EXECUTABLE go REQUIRED)
  endif()
endif()
message(STATUS "Go executable: ${KTAILCTL_WRAPPER_GO_EXECUTABLE}")
execute_process(COMMAND ${KTAILCTL_WRAPPER_GO_EXECUTABLE} version OUTPUT_VARIABLE KTAILCTL_WRAPPER_GO_VERSION)
message(STATUS "Go version: ${KTAILCTL_WRAPPER_GO_VERSION}")

add_library(ktailctl_wrapper_logging SHARED logging.cpp)
target_link_libraries(ktailctl_wrapper_logging PUBLIC Qt6::Core)
target_compile_definitions(ktailctl_wrapper_logging PRIVATE QT_MESSAGELOGCONTEXT)
target_include_directories(ktailctl_wrapper_logging PRIVATE ${PROJECT_BINARY_DIR}/src)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/logging.go.in ${CMAKE_CURRENT_SOURCE_DIR}/logging.go @ONLY)

if(NOT KTAILCTL_FLATPAK_BUILD)
  add_custom_command(
    OUTPUT go.sum
    COMMAND ${KTAILCTL_WRAPPER_GO_EXECUTABLE} get .
    MAIN_DEPENDENCY go.mod
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
    COMMAND GOEXPERIMENT=cgocheck2 ${KTAILCTL_WRAPPER_GO_EXECUTABLE} build -v -trimpath -mod vendor -o
            ${KTAILCTL_WRAPPER_LIBRARY} -buildmode=c-archive wrapper.go options.go taildrop.go callbacks.go logging.go
    MAIN_DEPENDENCY wrapper.go
    DEPENDS go.mod
            go.sum
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
    COMMAND ${KTAILCTL_WRAPPER_GO_EXECUTABLE} build -v -trimpath -mod vendor -o ${KTAILCTL_WRAPPER_LIBRARY}
            -buildmode=c-archive wrapper.go options.go taildrop.go callbacks.go logging.go
    MAIN_DEPENDENCY wrapper.go
    DEPENDS go.mod
            go.sum
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
