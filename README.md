# KTailctl

> [!NOTE]
> KTailctl is currently undergoing a big refactoring and redesign to make it more maintainable, efficient and modern looking. This effort is already quite advanced but will still take some time to complete. During this time, there will not be a lot of activity on the `main` branch.

[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/f-koehler/KTailctl/ci.yml)](https://github.com/f-koehler/KTailctl/actions/workflows/ci.yml)
[![GitHub](https://img.shields.io/github/license/f-koehler/KTailctl)](https://github.com/f-koehler/KTailctl)

A GUI to monitor and manage Tailscale on your Linux desktop, built using KDE Frameworks and Kirigami2.

> :warning: To get the most out of KTailctl (sending/receiving files, changing Tailscale settings, …) make sure you are the operator `tailscale up --operator=$USER`!

[![Flathub](https://dl.flathub.org/assets/badges/flathub-badge-en.svg)](https://flathub.org/apps/org.fkoehler.KTailctl)

![Tailscale peer list in KTailctl](https://raw.githubusercontent.com/f-koehler/KTailctl/main/screenshots/peerlist.png)

## Features

- [x] Monitoring of tailscale status
- [x] Monitoring of total traffic
- [x] Monitoring of traffic per peer
- [x] Detailed peer information
- [x] Tray menu for quick access
- [x] Copying of IP addresses/DNS name
- [ ] Speed graphs
- [ ] Pinging of peers
- [x] Exit node management
- [x] Toggle tailscale status
- [x] Sending files
- [x] Receiving files
- [ ] Notification on tailscale status change
- [ ] Notification on peer addition/removal
- [x] Flatpak
- [ ] Localization

## Development

### Setup

#### NixOS

Make sure thate flake support is enabled (see [NixOS Wiki on Flakes](https://nixos.wiki/wiki/Flakes)).
All required dependencies will be made availabe by entering the devshell via `nix develop --no-pure-eval`.
This can be achieved automatically when using `direnv` with `direnv allow`.

#### Fedora

Install required packages via

```shell
sudo dnf install \
  cmake \
  extra-cmake-modules \
  gcc-c++ \
  golang \
  json-devel \
  kf6-breeze-icons-devel \
  kf6-kconfig-devel \
  kf6-kcoreaddons-devel \
  kf6-kdbusaddons-devel \
  kf6-kguiaddons-devel \
  kf6-ki18n-devel \
  kf6-kirigami-addons-devel \
  kf6-kirigami-devel \
  kf6-knotifications-devel \
  kf6-kwindowsystem-devel \
  kf6-qqc2-desktop-style \
  qt6-qtbase-devel \
  qt6-qtdeclarative-devel \
  qt6-qtsvg-devel
```

### Compiling KTailctl

1. Vendor go modules

```shell
pushd src/wrapper
go mod vendor
popd
```

2. Configure with CMake

```shell
cmake -Bbuild
```

3. Compile with CMake (change 16 to the number of thread you want to use)

```shell
cmake --build build --parallel 16
```

4. Run binary

```shell
./build/bin/ktailctl
```

### Create a release

1. Create a changelog file `changelog/vX.Y.Z.md`.
2. Add a new release to `org.fkoehler.ktailctl.metainfo.xml`.
3. Update the version in `CMakeLists.txt`
4. Create a commit called `Release vX.Y.Z.`.
5. Create a tag from changelog: `git tag -s -F changelog/vX.Y.Z.md vX.Y.Z`.
6. Push tag: `git push origin vX.Y.Z`.
