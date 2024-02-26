#!/bin/bash
set -euf -o pipefail

PACKAGES=(
    appstream
    argagg-dev
    automake
    autotools-dev
    build-essential
    cmake
    extra-cmake-modules
    fuse
    gettext
    kirigami2-dev
    libboost-all-dev
    libfuse-dev
    libgpgme-dev
    libkf5config-dev
    libkf5coreaddons-dev
    libkf5guiaddons-dev
    libkf5i18n-dev
    libkf5notifications-dev
    libqt5svg5-dev
    librsvg2-dev
    libtool
    nlohmann-json3-dev
    pkg-config
    qtbase5-dev
    qtdeclarative5-dev
    qtquickcontrols2-5-dev
)

apt-get update -y
apt-get install -y ${PACKAGES[@]}
