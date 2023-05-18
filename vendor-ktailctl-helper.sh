#!/usr/bin/env bash
cd ktailctl-helper
go mod vendor -v
VERSION=$(git describe --tags --match "v*" --dirty)
tar cfvz ktailctl-helper-vendor-${VERSION}.tar.gz vendor
mv ktailctl-helper-vendor-${VERSION}.tar.gz ..
rm -rf vendor