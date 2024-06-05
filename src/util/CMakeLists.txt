add_library(ktailctl_util STATIC
    util.cpp
)
target_include_directories(
    ktailctl_util PUBLIC
    ${CMAKE_BINARY_DIR}/lib
)
target_link_libraries(ktailctl_util PUBLIC
    Qt6::Core
    Qt6::Gui
    KF6::GuiAddons
    tailwrap
)