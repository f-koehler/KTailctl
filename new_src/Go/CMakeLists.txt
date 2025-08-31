configure_file(${CMAKE_CURRENT_SOURCE_DIR}/logging.go.in ${CMAKE_CURRENT_SOURCE_DIR}/logging.go @ONLY)
add_library(ktailctl_go_logging SHARED logging.cpp)
target_link_libraries(ktailctl_go_logging PUBLIC Qt6::Core)
target_compile_definitions(ktailctl_go_logging PRIVATE QT_MESSAGELOGCONTEXT)

ecm_qt_declare_logging_category(
  ktailctl_go_logging
  HEADER
  "logging_go.hpp"
  IDENTIFIER
  "Logging::Go"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Go"
  DEFAULT_SEVERITY
  Info)
