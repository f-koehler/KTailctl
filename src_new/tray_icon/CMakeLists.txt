ecm_qt_declare_logging_category(
        ktailctl
        HEADER
        "logging_tray_icon.hpp"
        IDENTIFIER
        "Logging::TrayIcon"
        CATEGORY_NAME
        "org.fkoehler.KTailctl.TrayIcon"
        DEFAULT_SEVERITY
        Info)

target_sources(ktailctl PRIVATE tray_icon.hpp
        tray_menu_accounts.hpp tray_menu_exit_nodes.hpp tray_menu_peers.hpp tray_menu_self.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/logging_tray_icon.cpp)
target_include_directories(ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
