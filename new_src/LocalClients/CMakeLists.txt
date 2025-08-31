add_library(ktailctl_local_clients SHARED)
ecm_add_qml_module(ktailctl_local_clients URI org.fkoehler.KTailctl.LocalClients GENERATE_PLUGIN_SOURCE)
target_sources(ktailctl_local_clients PRIVATE TailscaleClient.cpp TailscaleAccountsClient.cpp
                                              TailscaleSettingsClient.cpp)
target_include_directories(ktailctl_local_clients PUBLIC ${PROJECT_SOURCE_DIR}/include)
ecm_finalize_qml_module(ktailctl_local_clients)
install(TARGETS ktailctl_local_clients ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
