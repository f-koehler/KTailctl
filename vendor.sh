#!/usr/bin/env bash
cd tailctlpp
go mod vendor -v
VERSION=$(git describe --tags --match "v*" --dirty)
tar cfvz tailctlpp-${VERSION}.tar.gz vendor
mv tailctlpp-${VERSION}.tar.gz ..
rm -rf vendor