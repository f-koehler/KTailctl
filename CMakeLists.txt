cmake_minimum_required(VERSION 3.24)

project(
  ktailctl
  VERSION 0.1
  LANGUAGES C CXX)

option(KTAILCTL_FLATPAK_BUILD "Build for Flatpak" OFF)

include(FeatureSummary)

set(QT_MAJOR_VERSION 6)
set(QT6_MIN_VERSION 6.6)
set(KF6_MIN_VERSION 6.0)
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
find_package(Git REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)

# include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)
# include(KDEClangFormat)

# include(KDEGitCommitHooks) file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.hpp *.c)
# kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

# kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX TAILCTL VERSION_HEADER
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
find_package(
  KF6 ${KF6_MIN_VERSION} REQUIRED
  COMPONENTS Config
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

execute_process(
  COMMAND ${GIT_EXECUTABLE} describe --tags --dirty --match "v*"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE KTAILCTL_GIT_DESCRIBE
  RESULT_VARIABLE KTAILCTL_GIT_DESCRIBE_RETVAL
  OUTPUT_STRIP_TRAILING_WHITESPACE)

if(NOT ${KTAILCTL_GIT_DESCRIBE_RETVAL} EQUAL 0)
  set(KTAILCTL_VERSION "unknown")
else()
  set(KTAILCTL_VERSION ${KTAILCTL_GIT_DESCRIBE})
endif()

message(STATUS "Version from git: ${KTAILCTL_VERSION}")
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/version.h @ONLY)

install(PROGRAMS org.fkoehler.KTailctl.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.fkoehler.KTailctl.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES KTailctl.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})
install(FILES org.fkoehler.KTailctl.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
