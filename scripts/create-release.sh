#!/bin/bash

TAG=$(git describe --tags --abbrev=0)
echo "Create release ${TAG}"

gh release create -F changelog/${TAG}.md --verify-tag ${TAG}
