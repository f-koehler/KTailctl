#!/bin/bash
set -euf -o pipefail

PACKAGES=(
    appstream
    build-essential
    cmake
    extra-cmake-modules
    fuse
    gettext
    kirigami2-dev
    libkf6config-dev
    libkf6coreaddons-dev
    libkf6guiaddons-dev
    libkf6i18n-dev
    libkf6notifications-dev
    libqt6quickcontrols2-6
    libqt6svg6-dev
    pkg-config
    qt6-base-dev
    qt6-declarative-dev
)

apt-get update -y
apt-get install -y ${PACKAGES[@]}
