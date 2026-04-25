file(GLOB _icon_files
     LIST_DIRECTORIES false
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     CONFIGURE_DEPENDS
     *.svg
)

foreach(_icon_file IN LISTS _icon_files)
  set_source_files_properties(${_icon_file} PROPERTIES QT_RESOURCE_ALIAS
          "country-flag-${_icon_file}")
endforeach()

qt_add_resources(ktailctl "flags" PREFIX "/country-flags" FILES ${_icon_files})
