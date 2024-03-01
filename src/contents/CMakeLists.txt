set(ICON_SIZES 16 18 20 22 24 26 28 30 32 40 48 56 64 80 96 112 128 160 192 224 256 320 384 448 512)
set(ICON_NAMES offline-breeze-dark offline-breeze-light offline-colorful online-breeze-dark online-breeze-light online-colorful)

add_subdirectory(icons/ktailctl)

set(ICON_SIZES_STR "${ICON_SIZES}")
set(ICON_NAMES_STR "${ICON_NAMES}")
string(REPLACE ";" "," ICON_SIZES_STR "${ICON_SIZES_STR}")
string(REPLACE ";" "," ICON_NAMES_STR "${ICON_NAMES_STR}")

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/icons.qrc
    COMMAND ${Python3_EXECUTABLE} ARGS ${CMAKE_CURRENT_SOURCE_DIR}/generate_resource.py ${CMAKE_CURRENT_SOURCE_DIR}/icons.qrc "${ICON_SIZES_STR}" "${ICON_NAMES_STR}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/generate_resource.py
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/icons/ktailctl/index.theme
)

add_custom_target(rasterized_icons ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/icons.qrc)
