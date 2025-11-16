# SPDX-License-Identifier: BSD-3-Clause SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)

add_subdirectory(ui/components)
add_subdirectory(wrapper)

add_executable(
  ktailctl
  about.cpp
  account_data.cpp
  account_model.cpp
  app.cpp
  exit_node_model.cpp
  json.cpp
  logging.cpp
  main.cpp
  mullvad_country_model.cpp
  mullvad_node_model.cpp
  peer_data.cpp
  peer_model.cpp
  preferences.cpp
  speed_statistics.cpp
  statistics.cpp
  status_data.cpp
  taildrop_receiver.cpp
  taildrop_sender.cpp
  tailscale.cpp
  util.cpp)

ecm_add_qml_module(ktailctl URI org.fkoehler.KTailctl GENERATE_PLUGIN_SOURCE)
ecm_target_qml_sources(
  ktailctl
  SOURCES
  ui/About.qml
  ui/Accounts.qml
  ui/ExitNodes.qml
  ui/Main.qml
  ui/MullvadNodes.qml
  ui/Peer.qml
  ui/Peers.qml
  ui/Self.qml
  ui/Settings.qml
  ui/Statistics.qml
  ui/TrayMenu.qml
)

ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_accounts_data.hpp"
  IDENTIFIER
  "Logging::AccountsData"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.AccountsData"
  DEFAULT_SEVERITY
  Info)
ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_taildrop_send.hpp"
  IDENTIFIER
  "Logging::TaildropSend"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.TaildropSend"
  DEFAULT_SEVERITY
  Info)
ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_peer_data.hpp"
  IDENTIFIER
  "Logging::PeerData"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.PeerData"
  DEFAULT_SEVERITY
  Info)
ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_statistics.hpp"
  IDENTIFIER
  "Logging::Statistics"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Statistics"
  DEFAULT_SEVERITY
  Info)
ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_status_data.hpp"
  IDENTIFIER
  "Logging::StatusData"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.StatusData"
  DEFAULT_SEVERITY
  Info)
ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_taildrop_receive.hpp"
  IDENTIFIER
  "Logging::TaildropReceive"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.TaildropReceive"
  DEFAULT_SEVERITY
  Info)
ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_wrapper.hpp"
  IDENTIFIER
  "Logging::Wrapper"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Wrapper"
  DEFAULT_SEVERITY
  Info)
ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_app.hpp"
  IDENTIFIER
  "Logging::App"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.App"
  DEFAULT_SEVERITY
  Info)

target_link_libraries(
  ktailctl
  PRIVATE KF6::BreezeIcons
          KF6::I18n
          KF6::DBusAddons
          KF6::ConfigCore
          KF6::ConfigGui
          KF6::CoreAddons
          KF6::WindowSystem
          KF6::GuiAddons
          Qt6::Widgets
          Qt6::Quick
          Qt6::QuickControls2
          KTailctl::WrapperLogging
          KTailctl::Wrapper)
target_compile_definitions(ktailctl PRIVATE WITH_BREEZEICONS_LIB)

set_source_files_properties(icons/logo.svg PROPERTIES QT_RESOURCE_ALIAS icons/logo.svg)
set_source_files_properties(icons/offline-adwaita-dark.svg PROPERTIES QT_RESOURCE_ALIAS icons/offline-adwaita-dark.svg)
set_source_files_properties(icons/offline-adwaita-light.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                       icons/offline-adwaita-light.svg)
set_source_files_properties(icons/offline-breeze-dark.svg PROPERTIES QT_RESOURCE_ALIAS icons/offline-breeze-dark.svg)
set_source_files_properties(icons/offline-breeze-light.svg PROPERTIES QT_RESOURCE_ALIAS icons/offline-breeze-light.svg)
set_source_files_properties(icons/offline-catppuccin-frappe.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                           icons/offline-catppuccin-frappe.svg)
set_source_files_properties(icons/offline-catppuccin-latte.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                          icons/offline-catppuccin-latte.svg)
set_source_files_properties(icons/offline-catppuccin-macchiato.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                              icons/offline-catppuccin-macchiato.svg)
set_source_files_properties(icons/offline-catppuccin-mocha.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                          icons/offline-catppuccin-mocha.svg)
set_source_files_properties(icons/offline-colorful.svg PROPERTIES QT_RESOURCE_ALIAS icons/offline-colorful.svg)
set_source_files_properties(icons/online-adwaita-dark.svg PROPERTIES QT_RESOURCE_ALIAS icons/online-adwaita-dark.svg)
set_source_files_properties(icons/online-adwaita-light.svg PROPERTIES QT_RESOURCE_ALIAS icons/online-adwaita-light.svg)
set_source_files_properties(icons/online-breeze-dark.svg PROPERTIES QT_RESOURCE_ALIAS icons/online-breeze-dark.svg)
set_source_files_properties(icons/online-breeze-light.svg PROPERTIES QT_RESOURCE_ALIAS icons/online-breeze-light.svg)
set_source_files_properties(icons/online-catppuccin-frappe.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                          icons/online-catppuccin-frappe.svg)
set_source_files_properties(icons/online-catppuccin-latte.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                         icons/online-catppuccin-latte.svg)
set_source_files_properties(icons/online-catppuccin-macchiato.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                             icons/online-catppuccin-macchiato.svg)
set_source_files_properties(icons/online-catppuccin-mocha.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                         icons/online-catppuccin-mocha.svg)
set_source_files_properties(icons/online-colorful.svg PROPERTIES QT_RESOURCE_ALIAS icons/online-colorful.svg)
set_source_files_properties(icons/exit-node-adwaita-dark.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                        icons/exit-node-adwaita-dark.svg)
set_source_files_properties(icons/exit-node-adwaita-light.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                         icons/exit-node-adwaita-light.svg)
set_source_files_properties(icons/exit-node-breeze-dark.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                       icons/exit-node-breeze-dark.svg)
set_source_files_properties(icons/exit-node-breeze-light.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                        icons/exit-node-breeze-light.svg)
set_source_files_properties(icons/exit-node-catppuccin-frappe.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                             icons/exit-node-catppuccin-frappe.svg)
set_source_files_properties(icons/exit-node-catppuccin-latte.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                            icons/exit-node-catppuccin-latte.svg)
set_source_files_properties(icons/exit-node-catppuccin-macchiato.svg
                            PROPERTIES QT_RESOURCE_ALIAS icons/exit-node-catppuccin-macchiato.svg)
set_source_files_properties(icons/exit-node-catppuccin-mocha.svg PROPERTIES QT_RESOURCE_ALIAS
                                                                            icons/exit-node-catppuccin-mocha.svg)
set_source_files_properties(icons/exit-node-colorful.svg PROPERTIES QT_RESOURCE_ALIAS icons/exit-node-colorful.svg)

include(./CountryFlags.cmake)
qt_add_resources(
  ktailctl
  "icons"
  FILES
  icons/logo.svg
  icons/offline-adwaita-dark.svg
  icons/offline-adwaita-light.svg
  icons/offline-breeze-dark.svg
  icons/offline-breeze-light.svg
  icons/offline-catppuccin-frappe.svg
  icons/offline-catppuccin-latte.svg
  icons/offline-catppuccin-macchiato.svg
  icons/offline-catppuccin-mocha.svg
  icons/offline-colorful.svg
  icons/online-adwaita-dark.svg
  icons/online-adwaita-light.svg
  icons/online-breeze-dark.svg
  icons/online-breeze-light.svg
  icons/online-catppuccin-frappe.svg
  icons/online-catppuccin-latte.svg
  icons/online-catppuccin-macchiato.svg
  icons/online-catppuccin-mocha.svg
  icons/online-colorful.svg
  icons/exit-node-adwaita-dark.svg
  icons/exit-node-adwaita-light.svg
  icons/exit-node-breeze-dark.svg
  icons/exit-node-breeze-light.svg
  icons/exit-node-catppuccin-frappe.svg
  icons/exit-node-catppuccin-latte.svg
  icons/exit-node-catppuccin-macchiato.svg
  icons/exit-node-catppuccin-mocha.svg
  icons/exit-node-colorful.svg)
qt_add_resources(ktailctl "flags" PREFIX "/country-flags" FILES ${KTAILCTL_FLAG_FILES})

kconfig_target_kcfg_file(
  ktailctl
  FILE
  ktailctlconfig.kcfg
  CLASS_NAME
  KTailctlConfig
  MUTATORS
  DEFAULT_VALUE_GETTERS
  GENERATE_PROPERTIES
  PARENT_IN_CONSTRUCTOR
  SINGLETON
  NOTIFIERS
  USE_ENUM_TYPES
  GENERATE_MOC
  QML_REGISTRATION)

target_include_directories(ktailctl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src
                                            ${CMAKE_BINARY_DIR}/lib)
target_link_directories(ktailctl PRIVATE ${CMAKE_BINARY_DIR}/lib)
add_dependencies(ktailctl ktailctl_wrapper_target)

if(KTAILCTL_FLATPAK_BUILD)
    target_compile_definitions(ktailctl PRIVATE KTAILCTL_FLATPAK_BUILD)
endif()

target_compile_definitions(ktailctl PRIVATE QT_MESSAGELOGCONTEXT)

install(TARGETS ktailctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

if(ipo_supported)
    set_property(TARGET ktailctl PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(APPLE)
    kirigami_package_breeze_icons(
    ICONS
    application-exit
    applications-graphics
    computer
    document-send
    edit-copy
    help-about
    list-add
    media-playback-start
    proces-stop
    question
    smartphone
    vcs-pull
    vcs-push
    view-refresh
    window)
endif()

add_dependencies(ktailctl ktailctl_components)

ecm_finalize_qml_module(ktailctl DESTINATION ${KDE_INSTALL_QMLDIR})

install(TARGETS ktailctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
