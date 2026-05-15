#!/bin/bash
set -euf -o pipefail

PACKAGES=(
	appstream
	build-essential
	clang
	cmake
	extra-cmake-modules
	gettext
	kf6-breeze-icon-theme-dev
	kf6-kirigami-addons-dev
	kf6-kirigami-dev
	libkf6config-dev
	libkf6coreaddons-dev
	libkf6dbusaddons-dev
	libkf6guiaddons-dev
	libkf6i18n-dev
	libkf6notifications-dev
	libkf6windowsystem-dev
	libomp-dev
	libqt6quickcontrols2-6
	libqt6svg6-dev
	ninja-build
	pkg-config
	qml6-module-org-kde-kirigami-addons-delegates
	qt6-base-dev
	qt6-declarative-dev
)

apt-get update -y
apt-get install -y ${PACKAGES[@]}
