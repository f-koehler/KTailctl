cmake_minimum_required(VERSION 3.31)

project(
  ktailctl
  VERSION 0.22.0
  LANGUAGES C CXX)

option(KTAILCTL_FLATPAK_BUILD "Build for Flatpak" OFF)
option(KTAILCTL_ENABLE_TESTS "Enable unit tests" OFF)
option(KTAILCTL_FETCH_GO "Fetch correct version of go" OFF)
option(KTAILCTL_ENABLE_ASAN "Enable Clang's address sanitizer" OFF)
option(KTAILCTL_ENABLE_UBSAN "Enable Clang's undefined behavior sanitizer" OFF)

if(KTAILCTL_ENABLE_ASAN OR KTAILCTL_ENABLE_UBSAN)
  if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "Sanitizers are only available when using Clang")
  endif()

  add_link_options(-shared-libsan)

  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} --print-runtime-dir
    OUTPUT_VARIABLE _clang_rt_lib_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE _clang_rt_result)
  if(_clang_rt_result OR NOT _clang_rt_lib_dir OR NOT EXISTS "${_clang_rt_lib_dir}")
    message(
      FATAL_ERROR
        "Could not locate the shared clang sanitizer runtime. "
        "On Debian/Ubuntu install libclang-rt-dev.")
  endif()
  message(STATUS "Clang sanitizer runtime directory: ${_clang_rt_lib_dir}")
  add_link_options("-Wl,-rpath,${_clang_rt_lib_dir}")
endif()

if(KTAILCTL_ENABLE_ASAN)
  message(STATUS "Enabling address sanitizer")
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address)
endif()

if(KTAILCTL_ENABLE_UBSAN)
  message(STATUS "Enabling undefined behavior sanitizer")
  add_compile_options(-fsanitize=undefined)
  add_link_options(-fsanitize=undefined)
endif()

set(QT_MAJOR_VERSION 6)
set(QT6_MIN_VERSION 6.10.0)
set(KF6_MIN_VERSION 6.24.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} \
    -Wall \
    -Wextra \
    -pedantic \
    -Wno-c99-extensions \
    -Wno-c++98-compat \
")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

set(KDE_COMPILERSETTINGS_LEVEL 6.24)

include(FeatureSummary)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(ECMGenerateExportHeader)
include(KDEInstallDirs)
include(ECMFindQmlModule)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMAddAppIcon)
include(ECMDeprecationSettings)
include(ECMQmlModule)

ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX KTAILCTL VERSION_HEADER
                  "${CMAKE_CURRENT_BINARY_DIR}/src/version-ktailctl.h")

find_package(
  Qt6 ${QT6_MIN_VERSION} REQUIRED
  COMPONENTS Core
             Gui
             Qml
             Quick
             QuickControls2
             Svg
             Widgets)

qt6_policy(SET QTP0001 NEW)
if(QT_KNOWN_POLICY_QTP0004)
  qt6_policy(SET QTP0004 NEW)
endif()

find_package(
  KF6 ${KF6_MIN_VERSION} REQUIRED
  COMPONENTS BreezeIcons
             Config
             CoreAddons
             DBusAddons
             GuiAddons
             I18n
             IconThemes
             Kirigami
             Notifications
             WindowSystem)

add_subdirectory(src)

add_custom_target(run
  COMMAND ${CMAKE_COMMAND} -E env
          "LSAN_OPTIONS=suppressions=${CMAKE_SOURCE_DIR}/tests/lsan.supp:print_suppressions=0"
          $<TARGET_FILE:ktailctl>
  DEPENDS ktailctl
  USES_TERMINAL
  COMMENT "Launching ktailctl with LSan suppressions")

if(KTAILCTL_ENABLE_TESTS)
  add_subdirectory(tests)
endif()

install(PROGRAMS org.fkoehler.KTailctl.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.fkoehler.KTailctl.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES org.fkoehler.KTailctl.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
