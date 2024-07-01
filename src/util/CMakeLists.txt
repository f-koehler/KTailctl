add_library(ktailctl_util STATIC util.cpp)
target_link_libraries(ktailctl_util PUBLIC Qt6::Core Qt6::Gui KF6::GuiAddons
                                           KTailctl::Wrapper KTailctl::Common)
add_library(KTailctl::Util ALIAS ktailctl_util)
