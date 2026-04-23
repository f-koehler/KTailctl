ecm_qt_declare_logging_category(
        ktailctl
        HEADER
        "logging_tailscale_preferences.hpp"
        IDENTIFIER
        "Logging::Tailscale::Preferences"
        CATEGORY_NAME
        "org.fkoehler.KTailctl.Tailscale.Preferences"
        DEFAULT_SEVERITY
        Info)
target_sources(ktailctl PRIVATE app_connector.hpp app_connector.cpp auto_update.hpp auto_update.cpp preferences.hpp preferences.cpp)
target_include_directories(ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
