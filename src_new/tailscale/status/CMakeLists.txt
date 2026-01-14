ecm_qt_declare_logging_category(
  new_ktailctl
  HEADER
  "logging_tailscale_status.hpp"
  IDENTIFIER
  "Logging::Tailscale::Status"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Tailscale.Status"
  DEFAULT_SEVERITY
  Info)

target_sources(
  new_ktailctl
  PRIVATE client_version.hpp
          exit_node_status.hpp
          location.hpp
          peer_status.hpp
          status.hpp
          tailnet_status.hpp
          user_profile.hpp
        login_profile.hpp
        network_profile.hpp
          ${CMAKE_CURRENT_BINARY_DIR}/logging_tailscale_status.cpp)
target_include_directories(new_ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
