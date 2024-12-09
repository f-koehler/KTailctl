#!/bin/bash
set -euf -o pipefail
flatpak remote-add --user --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
flatpak install --user -y flathub org.flatpak.Builder
flatpak install --user -y flathub org.flathub.flatpak-external-data-checker
flatpak install --user -y flathub org.kde.Sdk//6.8 org.kde.Platform/x86_64/6.8
flatpak install --user -y flathub org.freedesktop.Sdk.Extension.golang/x86_64/24.08
