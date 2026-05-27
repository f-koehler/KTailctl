#!/bin/bash

TAG=$(git describe --tags --abbrev=0)
echo "Create release ${TAG}"

if [[ "${TAG}" =~ -beta[0-9]+$ ]]; then
	gh release create \
		--prerelease \
		--verify-tag \
		--notes "This is a pre-release build for testing purposes only. It may contain bugs and is not intended for production use." \
		"${TAG}"
else
	gh release create \
		--verify-tag \
		-F "changelog/${TAG}.md" \
		"${TAG}"
fi

# Attach vendored go dependencies
./scripts/vendor.sh "${TAG}"
gh release upload "${TAG}" "ktailctl-wrapper-vendor-${TAG}.tar.gz"
