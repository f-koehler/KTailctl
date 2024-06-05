# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)

add_subdirectory(common)
add_subdirectory(data)
add_subdirectory(models)
add_subdirectory(config)
add_subdirectory(taildrop)
add_subdirectory(util)

add_executable(ktailctl
    about.cpp
    app.cpp
    logging.cpp
    main.cpp
    speed_statistics.cpp
    statistics.cpp
    tailscale.cpp
    tray_icon.cpp
    resources.qrc
)

target_link_libraries(ktailctl
    KF6::I18n
    Qt6::Quick
    Qt6::QuickControls2
    ktailctl_data
    ktailctl_models
    ktailctl_config
    ktailctl_taildrop
)

target_include_directories(
    ktailctl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/src
    ${CMAKE_BINARY_DIR}/lib
)
target_link_directories(ktailctl PRIVATE ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(ktailctl tailwrap_logging_wrapper tailwrap)
add_dependencies(ktailctl tailwrap_target)

if(KTAILCTL_FLATPAK_BUILD)
    target_compile_definitions(ktailctl PRIVATE KTAILCTL_FLATPAK_BUILD)
endif()

target_compile_definitions(ktailctl PRIVATE QT_MESSAGELOGCONTEXT)

install(TARGETS ktailctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

if(ipo_supported)
    set_property(TARGET ktailctl PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(APPLE)
    kirigami_package_breeze_icons(ICONS
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
        window
    )
endif()

add_subdirectory(Components)
add_dependencies(ktailctl mycomponentsplugin)