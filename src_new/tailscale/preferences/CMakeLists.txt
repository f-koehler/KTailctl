ecm_qt_declare_logging_category(
        new_ktailctl
        HEADER
        "logging_tailscale_preferences.hpp"
        IDENTIFIER
        "Logging::Tailscale::Preferences"
        CATEGORY_NAME
        "org.fkoehler.KTailctl.Tailscale.Preferences"
        DEFAULT_SEVERITY
        Info)
target_sources(new_ktailctl PRIVATE app_connector.hpp auto_update.hpp preferences.hpp)
target_include_directories(new_ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
