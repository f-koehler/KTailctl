ecm_add_qml_module(ktailctl_components URI "org.fkoehler.KTailctl.Components" GENERATE_PLUGIN_SOURCE)
ecm_target_qml_sources(
  ktailctl_components
  SOURCES
  FormCopyChipsDelegate.qml
  FormCopyLabelDelegate.qml
  FormLabeledIconDelegate.qml
  FormLabelDelegate.qml
  HeaderBanner.qml
  PeerInfoPage.qml)
ecm_finalize_qml_module(ktailctl_components DESTINATION ${KDE_INSTALL_QMLDIR})
