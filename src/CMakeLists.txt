# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)

add_executable(tailctl
    main.cpp
    about.cpp
    app.cpp
    status.cpp
    peer.cpp
    peer_model.cpp
    tailscale_client.cpp
    resources.qrc
)

target_link_libraries(tailctl
    KF5::Auth
    KF5::AuthCore
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::CoreAddons
    KF5::I18n
    KF5::Notifications
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Svg
)

kconfig_add_kcfg_files(tailctl GENERATE_MOC tailctlconfig.kcfgc)
install(TARGETS tailctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

add_executable(tailctl_tailscale_helper
    tailscale_helper.cpp
)
target_link_libraries(tailctl_tailscale_helper
    KF5::AuthCore
    Qt5::Network
)
install(TARGETS tailctl_tailscale_helper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(tailctl_tailscale_helper org.fkoehler.tailctl root)
kauth_install_actions(org.fkoehler.tailctl org.fkoehler.tailctl.actions)

add_executable(tailctl_taildrop_helper
    taildrop_helper.cpp
)
target_link_libraries(tailctl_taildrop_helper
    Qt5::Core
    Qt5::Network
)
install(TARGETS tailctl_taildrop_helper ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

if(ipo_supported)
    set_property(TARGET tailctl PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET tailctl_tailscale_helper PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET tailctl_taildrop_helper PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()