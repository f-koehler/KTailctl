include(ECMQmlModule)

ecm_add_qml_module(mycomponentsplugin URI "org.fkoehler.KTailctl.Components" VERSION 1.0)
target_sources(mycomponentsplugin PRIVATE
    componentsplugin.cpp
)
ecm_target_qml_sources(mycomponentsplugin SOURCES
    FormCopyChipsDelegate.qml
    FormCopyLabelDelegate.qml
    FormLabeledIconDelegate.qml
    FormLabelDelegate.qml
    PeerInfo.qml
)
target_link_libraries(mycomponentsplugin PRIVATE
    Qt6::Quick
    Qt6::Qml
    KF6::I18n
    KF6::GuiAddons
)
ecm_finalize_qml_module(mycomponentsplugin DESTINATION ${KDE_INSTALL_QMLDIR})
