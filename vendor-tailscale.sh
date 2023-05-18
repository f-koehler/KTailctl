#!/usr/bin/env bash
TAILSCALE_VERSION=1.40.1
git clone --depth 1 --branch v${TAILSCALE_VERSION} https://github.com/tailscale/tailscale tailscale
cd tailscale
go mod vendor -v
tar cfvz tailscale-vendor-${TAILSCALE_VERSION}.tar.gz vendor
mv tailscale-vendor-${TAILSCALE_VERSION}.tar.gz ../
