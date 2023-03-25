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
    taildrop_process.cpp
    resources.qrc
)

target_link_libraries(tailctl
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

if(ipo_supported)
    set_property(TARGET tailctl PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()