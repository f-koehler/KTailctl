#!/usr/bin/env bash
pushd src/wrapper
rm -rf vendor
go get -v .
go mod tidy -v
go mod vendor -v
VERSION=$(git describe --tags --match "v*" --dirty)
tar cfvz ktailctl-wrapper-vendor-${VERSION}.tar.gz vendor
mv ktailctl-wrapper-vendor-${VERSION}.tar.gz ../../
popd
