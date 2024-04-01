# KTailctl

[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/f-koehler/KTailctl/ci.yml)](https://github.com/f-koehler/KTailctl/actions/workflows/ci.yml)
[![GitHub](https://img.shields.io/github/license/f-koehler/KTailctl)](https://github.com/f-koehler/KTailctl)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/f-koehler/KTailctl/main.svg)](https://results.pre-commit.ci/latest/github/f-koehler/KTailctl/main)

A GUI to monitor and manage Tailscale on your Linux desktop, built using KDE Frameworks and Kirigami2.

> :warning: To get the most out of KTailctl (sending/receiving files, changing Tailscale settings, …) make sure you are the operator `tailscale up --operator=$USER`!

[![Flathub](https://dl.flathub.org/assets/badges/flathub-badge-en.svg)](https://flathub.org/apps/org.fkoehler.KTailctl)

![Main Window](https://raw.githubusercontent.com/f-koehler/KTailctl/main/screenshots/main.png)

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

## Developing

### Create a release

1. Create a changelog file `changelog/vX.Y.Z.md`.
2. Add a new release to `org.fkoehler.ktailctl.metainfo.xml`.
3. Create a commit called `Release vX.Y.Z.`.
4. Create a tag from changelog: `git tag -s -F changelog/vX.Y.Z.md vX.Y.Z`.
5. Push tag: `git push origin vX.Y.Z`.

### Install Dependencies using craft

```bash
craft kirigami ki18n kconfig knotifications kcoreaddons qtquickcontrols qqc2-desktop-style
```
