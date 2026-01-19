#!/bin/bash
set -euf -o pipefail

PACKAGES=(
    extra-cmake-modules
    qt6-qtbase-devel
    qt6-qtdeclarative-devel
    qt6-qtquickcontrols2-devel
    qt6-qtsvg-devel
    kf6-kconfig-devel
    kf6-kcoreaddons-devel
    kf6-kguiaddons-devel
    kf6-ki18n-devel
    kf6-kirigami2-devel
    kf6-knotifications-devel
    kf6-kdbusaddons-devel
    kf6-kwindowsystem-devel
    kf6-breeze-icons-devel
    json-devel
)

dnf install -y "${PACKAGES[@]}"
