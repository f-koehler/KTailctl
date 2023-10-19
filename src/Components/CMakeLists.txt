include(ECMQmlModule)

ecm_add_qml_module(componentsplugin URI "org.fkoehler.KTailctl.Components" VERSION 1.0)
target_sources(componentsplugin PRIVATE
    componentsplugin.cpp
)
ecm_target_qml_sources(componentsplugin SOURCES
    FormCopyLabelDelegate.qml
)
target_link_libraries(componentsplugin PRIVATE
    Qt5::Quick
    Qt5::Qml
    KF5::I18n
    KF5::GuiAddons
)
ecm_finalize_qml_module(componentsplugin DESTINATION ${KDE_INSTALL_QMLDIR})