add_library(ktailctl_taildrop STATIC taildrop_receiver.cpp taildrop_sender.cpp)
target_link_libraries(ktailctl_taildrop Qt6::Widgets KF6::CoreAddons
                      KTailctl::Config KTailctl::Util KTailctl::Wrapper)
add_library(KTailctl::Taildrop ALIAS ktailctl_taildrop)
