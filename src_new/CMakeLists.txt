ecm_qt_declare_logging_category(
        new_ktailctl
        HEADER
        "logging_main.hpp"
        IDENTIFIER
        "Logging::Main"
        CATEGORY_NAME
        "org.fkoehler.KTailctl"
        DEFAULT_SEVERITY
        Info)

add_executable(new_ktailctl peer.hpp property_list_model.hpp main.cpp
        util.hpp
        tailscale/preferences/app_connector.hpp
)
target_link_libraries(new_ktailctl PRIVATE Qt6::Core Qt6::Quick Qt6::Widgets KF6::CoreAddons KF6::I18n KF6::DBusAddons KF6::GuiAddons)
ecm_add_qml_module(new_ktailctl URI org.fkoehler.KTailctl GENERATE_PLUGIN_SOURCE)
ecm_target_qml_sources(new_ktailctl SOURCES ui/Main.qml)
install(TARGETS new_ktailctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(tailscale)
add_subdirectory(ui)
add_subdirectory(icons)
add_subdirectory(tray_icon)
