cmake_minimum_required(VERSION 3.24)

project(
  ktailctl
  VERSION 0.21.5
  LANGUAGES C CXX)

option(KTAILCTL_FLATPAK_BUILD "Build for Flatpak" OFF)

set(QT_MAJOR_VERSION 6)
set(QT6_MIN_VERSION 6.5.0)
set(KF6_MIN_VERSION 6.5.0)
set(CMAKE_CXX_STANDARD 20)
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

set(KDE_COMPILERSETTINGS_LEVEL 6.0)
# add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_NO_URL_CAST_FROM_STRING
# -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT -DQT_USE_QSTRINGBUILDER )

include(FeatureSummary)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(ECMGenerateExportHeader)
include(KDEInstallDirs)
include(ECMFindQmlModule)
include(KDECMakeSettings)
# include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddAppIcon)
include(ECMDeprecationSettings)
include(ECMQmlModule)

ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX KTAILCTL VERSION_HEADER
                  "${CMAKE_CURRENT_BINARY_DIR}/src/version-ktailctl.h")

find_package(
  Qt6 ${QT6_MIN_VERSION} REQUIRED
  COMPONENTS Core
             Gui
             # Qml
             Quick
             QuickControls2
             Svg
             Test
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
             Kirigami
             Notifications
             WindowSystem)
find_package(nlohmann_json REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

install(PROGRAMS org.fkoehler.KTailctl.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.fkoehler.KTailctl.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES org.fkoehler.KTailctl.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
