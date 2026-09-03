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

# No-op on platforms other than Windows/macOS. On macOS this uses KIconThemes' ksvg2icns to turn
# the app logo into a proper .icns and sets MACOSX_BUNDLE_ICON_FILE, so the app bundle gets a real
# Dock/Finder icon instead of the generic default one. Must be called before add_executable():
# ecm_add_app_icon() only sets MACOSX_BUNDLE_ICON_FILE as a plain variable, which CMake's bundle
# Info.plist generation only picks up from the variable state at the time the target is created.
# Deliberately uses icons/logo.svg rather than org.fkoehler.KTailctl.svg (a symlink to the same
# file): ksvg2icns names its output after everything before the *first* dot in the source
# filename, while ecm_add_app_icon expects everything before the *last* one, so the reverse-DNS
# name would produce a mismatched org.icns that never gets picked up.
ecm_add_app_icon(ktailctl_SOURCES ICONS ${CMAKE_CURRENT_SOURCE_DIR}/icons/logo.svg)

add_executable(ktailctl property_list_model.hpp main.cpp
        util.hpp util.cpp
        string_filter.hpp string_filter.cpp
        tailscale/preferences/app_connector.hpp
        ${ktailctl_SOURCES}
)
target_link_libraries(ktailctl PRIVATE Qt6::Core Qt6::Quick Qt6::QuickControls2 Qt6::Widgets Qt6::Qml KF6::CoreAddons KF6::I18n KF6::I18nQml KF6::DBusAddons KF6::GuiAddons KF6::IconThemes KF6::BreezeIcons KF6::ColorScheme KF6::ConfigCore KF6::WindowSystem)

set_target_properties(ktailctl PROPERTIES
  MACOSX_BUNDLE_BUNDLE_NAME "KTailctl"
  MACOSX_BUNDLE_GUI_IDENTIFIER "org.fkoehler.KTailctl"
  MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
  MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
  MACOSX_BUNDLE_COPYRIGHT "© 2023 Fabian Koehler"
)

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
