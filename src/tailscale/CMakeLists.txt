add_library(ktailctl_tailscale STATIC location.cpp peer.cpp tailscale.cpp
                                      peer_model.cpp)
target_link_libraries(ktailctl_tailscale KTailctl::Data KTailctl::Wrapper)
add_library(KTailctl::Tailscale ALIAS ktailctl_tailscale)
