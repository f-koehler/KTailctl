add_executable(new_ktailctl peer.hpp property_list_model.hpp main.cpp
        util.hpp)
target_link_libraries(new_ktailctl PRIVATE Qt6::Core Qt6::Quick Qt6::Widgets KF6::CoreAddons KF6::I18n KF6::DBusAddons KF6::GuiAddons)
ecm_add_qml_module(new_ktailctl URI org.fkoehler.KTailctl GENERATE_PLUGIN_SOURCE)
ecm_target_qml_sources(new_ktailctl SOURCES ui/Main.qml)
install(TARGETS new_ktailctl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(tailscale)
add_subdirectory(ui)
add_subdirectory(icons)
