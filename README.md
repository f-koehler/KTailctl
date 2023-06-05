# KTailctl

[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/f-koehler/KTailctl/ci.yml)](https://github.com/f-koehler/KTailctl/actions/workflows/ci.yml)
[![GitHub](https://img.shields.io/github/license/f-koehler/KTailctl)](https://github.com/f-koehler/KTailctl)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/f-koehler/KTailctl/main.svg)](https://results.pre-commit.ci/latest/github/f-koehler/KTailctl/main)

A GUI to monitor and manage Tailscale on your Linux desktop, built using KDE Frameworks and Kirigami2.

[![Flathub](https://dl.flathub.org/assets/badges/flathub-badge-en.svg)](https://flathub.org/apps/org.fkoehler.KTailctl)

![Main Window](screenshots/main.png)

## Features

- [x] Monitoring of tailscale status
- [x] Monitoring of total traffic
- [x] Monitoring of traffic per peer
- [x] Detailed peer information
- [x] Tray menu for quick access
- [x] Copying of IP addresses/DNS name
- [ ] Speed graphs
- [ ] Pinging of peers
- [ ] Exit node management
- [x] Toggle tailscale status
- [ ] Sending files
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
6. Create GitHub release: `gh release create -F changelog/vX.Y.Z.md --verify-tag vX.Y.Z`
7. Vendor go dependencies: `./vendor.sh` (when go deps change)
8. Upload vendor tarball: `gh release upload vX.Y.Z tailctlpp-vX.Y.Z.tar.gz` (when go deps change)

### Install Dependencies using craft

```bash
craft kirigami ki18n kconfig knotifications kcoreaddons
```
