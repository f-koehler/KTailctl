#!/usr/bin/env bash
VERSION=${1:-$(git describe --tags --abbrev=0)}
DEST_DIR="$(pwd)"
pushd src/tailscale/wrapper || exit
rm -rf vendor
go get -v .
go mod tidy -v
go mod vendor -v
tar cfvz "${DEST_DIR}/ktailctl-wrapper-vendor-${VERSION}.tar.gz" vendor
popd || exit
