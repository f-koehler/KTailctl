add_library(ktailctl_config preferences.cpp)
kconfig_add_kcfg_files(ktailctl_config GENERATE_MOC ktailctlconfig.kcfgc)
target_link_libraries(ktailctl_config KF6::ConfigCore KF6::ConfigGui
                      ktailctl_wrapper)
install(TARGETS ktailctl_config)
