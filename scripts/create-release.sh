#!/bin/bash

TAG=$(git describe --tags --abbrev=0)
echo "Create release ${TAG}"

# Create release
gh release create -F changelog/${TAG}.md --verify-tag ${TAG}

# Attach vendored go dependencies
./scripts/vendor.sh
gh release upload ${TAG} tailwrap-vendor-${TAG}.tar.gz

# Create an AppImage and attach it to the release
yq -i '.AppDir.app_info.version = "${TAG}"' AppImage/AppImageBuilder.yml
pushd AppImage
CC=clang-17 CXX=clang++-17 ./build.sh
gh release upload ${TAG} KTailctl-${TAG}-x86_64.AppImage
popd
