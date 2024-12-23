add_library(ktailctl_tailscale STATIC peer_model.cpp tailscale.cpp exit_node_model.cpp mullvad_country_model.cpp
                                      mullvad_node_model.cpp)
target_link_libraries(ktailctl_tailscale PUBLIC Qt6::Core KTailctl::Data KTailctl::Wrapper KTailctl::Config)
add_library(KTailctl::Tailscale ALIAS ktailctl_tailscale)
