#!/bin/bash
set -eu -o pipefail

source ./scripts/lib/goqtflags.sh

go install -v mvdan.cc/gofumpt@latest

cd src/wrapper
OUTPUT=$(~/go/bin/gofumpt -l -d -w $(ls -1 *.go))
if [ -n "${OUTPUT}" ]; then
    echo "${OUTPUT}"
    exit 1
fi

exit 0
