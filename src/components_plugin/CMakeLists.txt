include(ECMQmlModule)

ecm_add_qml_module(ktailctl_components_plugin URI "org.fkoehler.KTailctl.Components" VERSION 1.0)
target_sources(ktailctl_components_plugin PRIVATE
    components_plugin.cpp
)
ecm_target_qml_sources(ktailctl_components_plugin SOURCES
    FormCopyChipsDelegate.qml
    FormCopyLabelDelegate.qml
    FormLabeledIconDelegate.qml
    FormLabelDelegate.qml
    PeerInfoPage.qml
)
ecm_finalize_qml_module(ktailctl_components_plugin DESTINATION ${KDE_INSTALL_QMLDIR})
