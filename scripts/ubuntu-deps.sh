#!/bin/bash
set -euf -o pipefail

source ./scripts/lib/goqtflags.sh

PACKAGES=(
    appstream
    cmake
    extra-cmake-modules
    gettext
    kirigami2-dev
    libkf5config-dev
    libkf5coreaddons-dev
    libkf5guiaddons-dev
    libkf5i18n-dev
    libkf5notifications-dev
    libqt5svg5-dev
    qtbase5-dev
    qtdeclarative5-dev
    qtquickcontrols2-5-dev
)

apt-get update -y
apt-get install -y ${PACKAGES[@]}