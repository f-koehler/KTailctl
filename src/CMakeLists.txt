# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

add_executable(tailctl
    main.cpp
    about.cpp
    app.cpp
    status.cpp
    resources.qrc)

target_link_libraries(tailctl
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Svg
    KF5::I18n
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::ConfigGui
)

if (ANDROID)
    kirigami_package_breeze_icons(ICONS
        list-add
        help-about
        application-exit
        applications-graphics
        settings-configure
    )
endif()

kconfig_add_kcfg_files(tailctl GENERATE_MOC tailctlconfig.kcfgc)
install(TARGETS tailctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
