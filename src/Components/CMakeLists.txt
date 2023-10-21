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
    Qt5::Quick
    Qt5::Qml
    KF5::I18n
    KF5::GuiAddons
)
ecm_finalize_qml_module(mycomponentsplugin DESTINATION ${KDE_INSTALL_QMLDIR})
