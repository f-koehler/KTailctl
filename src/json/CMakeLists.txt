get_target_property(KTAILCTL_QMAKE Qt::qmake IMPORTED_LOCATION)
set(KTAILCTL_JSON_CRATE ktailctl-json)
corrosion_import_crate(MANIFEST_PATH ${CMAKE_CURRENT_LIST_DIR}/Cargo.toml CRATES ${KTAILCTL_JSON_CRATE})
set(KTAILCTL_CXXQT_EXPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/cxxqt")
corrosion_set_env_vars(${KTAILCTL_JSON_CRATE}
    "CXXQT_EXPORT_DIR=${KTAILCTL_CXXQT_EXPORT_DIR}"
    "QMAKE=${KTAILCTL_QMAKE}"
)
add_library(ktailctl_json_lib INTERFACE)
target_include_directories(ktailctl_json_lib INTERFACE "${KTAILCTL_CXXQT_EXPORT_DIR}/${KTAILCTL_JSON_CRATE}")
target_link_libraries(ktailctl_json_lib INTERFACE
    "$<LINK_LIBRARY:WHOLE_ARCHIVE,${KTAILCTL_JSON_CRATE}-static>"
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::QuickControls2
)