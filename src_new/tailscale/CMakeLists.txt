add_subdirectory(status)
add_subdirectory(wrapper)

ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_tailscale.hpp"
  IDENTIFIER
  "Logging::TailscaleMain"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Tailscale"
  DEFAULT_SEVERITY
  Info)

target_sources(new_ktailctl PRIVATE tailscale.hpp ${CMAKE_CURRENT_BINARY_DIR}/logging_tailscale.cpp)
target_include_directories(new_ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
