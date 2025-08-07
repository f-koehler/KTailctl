add_library(ktailctl_nodes SHARED)
ecm_add_qml_module(ktailctl_nodes URI org.fkoehler.KTailctl.Nodes GENERATE_PLUGIN_SOURCE)
target_sources(ktailctl_nodes PRIVATE Location.cpp NodeProperties.cpp TailscaleNetworkNode.cpp
                                      TailscaleNodeProperties.cpp)
target_include_directories(ktailctl_nodes PUBLIC ${PROJECT_SOURCE_DIR}/include)
ecm_finalize_qml_module(ktailctl_nodes)
install(TARGETS ktailctl_nodes ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
