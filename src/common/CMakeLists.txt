add_library(ktailctl_common STATIC location.cpp peer.cpp status.cpp)
target_link_libraries(ktailctl_common KTailctl::Data KTailctl::Wrapper)
add_library(KTailctl::Common ALIAS ktailctl_common)
