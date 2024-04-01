#!/bin/bash
set -euf -o pipefail
flatpak remote-add --user --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
flatpak install --user -y flathub org.flatpak.Builder
flatpak install --user -y flathub org.flathub.flatpak-external-data-checker
flatpak install --user -y flathub org.kde.Sdk//6.6 org.kde.Platform/x86_64/6.6
flatpak install --user -y flathub org.freedesktop.Sdk.Extension.golang/x86_64/23.08
flatpak install --user -y flathub runtime/org.freedesktop.Sdk.Extension.llvm17/x86_64/23.08
