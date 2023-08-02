#!/usr/bin/env bash
set -euf -o pipefail

GO_VERSION="1.20.7"

ARCH="$(arch | sed s/aarch64/arm64/ | sed s/x86_64/amd64/)"
wget -O go${GO_VERSION}.tar.gz "https://go.dev/dl/go${GO_VERSION}.linux-${ARCH}.tar.gz";
tar -C /opt -xzf go${GO_VERSION}.tar.gz
rm -f go${GO_VERSION}.tar.gz