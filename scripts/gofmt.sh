#!/bin/bash
set -eu -o pipefail

source ./scripts/lib/goqtflags.sh

cd src/wrapper
OUTPUT=$(gofmt -l -d -w $(ls -1 *.go))
if [ -n "${OUTPUT}" ]; then
    echo "${OUTPUT}"
    exit 1
fi

exit 0
