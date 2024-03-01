#!/bin/bash
set -euf -o pipefail

PACKAGES=(
    appstream
    cmake
    extra-cmake-modules
    gettext
    kirigami2-dev
    libkf6config-dev
    libkf6coreaddons-dev
    libkf6guiaddons-dev
    libkf6i18n-dev
    libkf6notifications-dev
    libqt6svg6-dev
    qt6-base-dev
    qt6-declarative-dev
    qt6-quickcontrols2-dev
    pkg-config
    build-essential
    fuse
)

apt-get update -y
apt-get install -y ${PACKAGES[@]}
