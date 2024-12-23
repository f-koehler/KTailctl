#!/bin/bash
set -euf -o pipefail

PACKAGES=(
    appstream
    build-essential
    cmake
    extra-cmake-modules
    fuse
    gettext
    kf6-kirigami-addons-dev
    kf6-kirigami-dev
    libkf6config-dev
    libkf6coreaddons-dev
    libkf6guiaddons-dev
    libkf6i18n-dev
    libkf6notifications-dev
    libkf6notifications-dev
    libkf6windowsystem-dev
    libkf6dbusaddons-dev
    libqt6quickcontrols2-6
    libqt6svg6-dev
    nlohmann-json3-dev
    pkg-config
    qt6-base-dev
    qt6-declarative-dev
    qml6-module-org-kde-kirigami-addons-delegates
)

apt-get update -y
apt-get install -y ${PACKAGES[@]}
