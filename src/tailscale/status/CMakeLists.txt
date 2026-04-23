ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_tailscale_status.hpp"
  IDENTIFIER
  "Logging::Tailscale::Status"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Tailscale.Status"
  DEFAULT_SEVERITY
  Info)

target_sources(
  ktailctl
  PRIVATE client_version.hpp client_version.cpp
          exit_node_status.hpp exit_node_status.cpp
          location.hpp location.cpp
          peer_status.hpp peer_status.cpp
          status.hpp status.cpp
          tailnet_status.hpp tailnet_status.cpp
          user_profile.hpp user_profile.cpp
          login_profile.hpp login_profile.cpp
          network_profile.hpp network_profile.cpp
          mullvad_exit_node_model.hpp mullvad_exit_node_model.cpp
          self_hosted_exit_node_model.hpp self_hosted_exit_node_model.cpp
          ${CMAKE_CURRENT_BINARY_DIR}/logging_tailscale_status.cpp)
target_include_directories(ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
