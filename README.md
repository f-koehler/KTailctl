# KTailctl

![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/f-koehler/KTailctl/ci.yml)

A GUI to monitor and manage Tailscale on your Linux desktop, built using KDE Frameworks and Kirigami2.

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
- [x] Creating flatpak (submitted to flathub)
- [ ] Localization

## Developing

### Create a release

1. Create a changelog file `changelog/vX.Y.Z.md`.
2. Create a commit called `Release vX.Y.Z.`.
3. Create a tag from changelog: `git tag -s -F changelog/vX.Y.Z.md vX.Y.Z`.
4. Push tag: `git push origin vX.Y.Z`.
5. Create GitHub release: `gh release create -F changelog/vX.Y.Z.md --verify-tag vX.Y.Z`
