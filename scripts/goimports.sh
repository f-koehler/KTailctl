#!/bin/bash
set -eu -o pipefail

source ./scripts/lib/goqtflags.sh

go install -v golang.org/x/tools/cmd/goimports@latest

cd tailwrap
OUTPUT=$(~/go/bin/goimports -l -d -w $(ls -1 *.go))
if [ -n "${OUTPUT}" ]; then
    echo "${OUTPUT}"
    exit 1
fi

exit 0
