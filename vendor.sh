#!/usr/bin/env bash
cd tailwrap
rm -rf vendor
go mod vendor -v
VERSION=$(git describe --tags --match "v*" --dirty)
tar cfvz tailwrap-vendor-${VERSION}.tar.gz vendor
mv tailwrap-vendor-${VERSION}.tar.gz ..
