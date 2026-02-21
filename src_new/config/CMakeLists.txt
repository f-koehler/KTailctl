find_package(KF6 REQUIRED COMPONENTS Config)
target_link_libraries(ktailctl PRIVATE KF6::ConfigCore KF6::ConfigGui)

ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_config.hpp"
  IDENTIFIER
  "Logging::Config"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Config"
  DEFAULT_SEVERITY
  Info)

target_sources(ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/logging_config.cpp)
target_include_directories(ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
