add_library(ktailctl_util STATIC util.cpp)
target_link_libraries(ktailctl_util PUBLIC Qt6::Core Qt6::Gui KF6::GuiAddons
                                           ktailctl_wrapper ktailctl_common)
