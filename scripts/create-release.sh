#!/bin/bash

TAG=$(git describe --tags --abbrev=0)
echo "Create release ${TAG}"

# Create release
gh release create -F changelog/${TAG}.md --verify-tag ${TAG}

# Attach vendored go dependencies
./scripts/vendor.sh
gh release upload ${TAG} tailwrap-vendor-${TAG}.tar.gz
