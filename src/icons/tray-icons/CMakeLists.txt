# cmake-format: off
set(KTAILCTL_TRAY_ICON_THEMES
        "breeze-light"         "#232629" "#da4453"
        "breeze-dark"          "#eff0f1" "#eff0f1"
        "colorful"             "#27ae60" "#da4453"
        "adwaita-dark"         "#26a269" "#ff7b63"
        "adwaita-light"        "#2ec27e" "#c01c28"
        "catppuccin-latte"     "#8839ef" "#8839ef"
        "catppuccin-frappe"    "#ca9ee6" "#ca9ee6"
        "catppuccin-macchiato" "#c6a0f6" "#c6a0f6"
        "catppuccin-mocha"     "#cba6f7" "#cba6f7"
)
# cmake-format: on

# generate a header listing all the tray icon themes
list(LENGTH KTAILCTL_TRAY_ICON_THEMES _len)
math(EXPR _last "${_len} - 1")
foreach(i RANGE 0 ${_last} 3)
    list(GET KTAILCTL_TRAY_ICON_THEMES ${i} _name)
    list(APPEND _theme_names "QStringLiteral(\"${_name}\")")
endforeach()
list(JOIN _theme_names ", " KTAILCTL_TRAY_ICON_THEMES_LITERAL_LIST)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/tray_icon_themes.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/tray_icon_themes.hpp
    @ONLY
)
target_sources(ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/tray_icon_themes.hpp)
target_include_directories(ktailctl PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


# generated themed icons
# generated themed icons
set(_tray_icon_files)

foreach(i RANGE 0 ${_last} 3)
    list(GET KTAILCTL_TRAY_ICON_THEMES ${i} _theme_name)

    math(EXPR online_color_index "${i} + 1")
    math(EXPR offline_color_index "${i} + 2")

    list(GET KTAILCTL_TRAY_ICON_THEMES ${online_color_index} _theme_online_color)
    list(GET KTAILCTL_TRAY_ICON_THEMES ${offline_color_index} _theme_offline_color)

    set(THEME_ONLINE_COLOR  "${_theme_online_color}")
    set(THEME_OFFLINE_COLOR "${_theme_offline_color}")

    set(_online_file     ${CMAKE_CURRENT_BINARY_DIR}/online-${_theme_name}.svg)
    set(_offline_file    ${CMAKE_CURRENT_BINARY_DIR}/offline-${_theme_name}.svg)
    set(_exit_node_file  ${CMAKE_CURRENT_BINARY_DIR}/exit-node-${_theme_name}.svg)

    configure_file(online.svg.in     ${_online_file}    @ONLY)
    configure_file(offline.svg.in    ${_offline_file}   @ONLY)
    configure_file(exit-node.svg.in  ${_exit_node_file} @ONLY)

    set_source_files_properties(${_online_file}
        PROPERTIES QT_RESOURCE_ALIAS online-${_theme_name}.svg)

    set_source_files_properties(${_offline_file}
        PROPERTIES QT_RESOURCE_ALIAS offline-${_theme_name}.svg)

    set_source_files_properties(${_exit_node_file}
        PROPERTIES QT_RESOURCE_ALIAS exit-node-${_theme_name}.svg)

    list(APPEND _tray_icon_files
        ${_online_file}
        ${_offline_file}
        ${_exit_node_file}
    )
endforeach()

qt_add_resources(ktailctl "tray-icons"
    PREFIX "/tray-icons"
    FILES ${_tray_icon_files}
)