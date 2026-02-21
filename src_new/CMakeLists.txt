ecm_qt_declare_logging_category(
        ktailctl
        HEADER
        "logging_main.hpp"
        IDENTIFIER
        "Logging::Main"
        CATEGORY_NAME
        "org.fkoehler.KTailctl"
        DEFAULT_SEVERITY
        Info)

add_executable(ktailctl peer.hpp property_list_model.hpp main.cpp
        util.hpp
        tailscale/preferences/app_connector.hpp
)
target_link_libraries(ktailctl PRIVATE Qt6::Core Qt6::Quick Qt6::Widgets KF6::CoreAddons KF6::I18n KF6::DBusAddons KF6::GuiAddons)
ecm_add_qml_module(ktailctl URI org.fkoehler.KTailctl GENERATE_PLUGIN_SOURCE)
ecm_target_qml_sources(ktailctl SOURCES ui/Main.qml)
install(TARGETS ktailctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(tailscale)
add_subdirectory(ui)
add_subdirectory(icons)
add_subdirectory(tray_icon)
add_subdirectory(config)

# Must be called here, build breaks if this is done inside config/CMakeLists.txt
kconfig_target_kcfg_file(ktailctl
  FILE ${CMAKE_CURRENT_SOURCE_DIR}/config/ktailctl_config.kcfg
  CLASS_NAME Config
  MUTATORS
  DEFAULT_VALUE_GETTERS
  GENERATE_PROPERTIES
  PARENT_IN_CONSTRUCTOR
  SINGLETON
  NOTIFIERS
  USE_ENUM_TYPES
  GENERATE_MOC
)
