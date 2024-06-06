#!/bin/bash
set -eu -o pipefail

source ./scripts/lib/goqtflags.sh

go install -v github.com/sqs/goreturns@latest

cd src/wrapper
OUTPUT=$(~/go/bin/goreturns -l -d -w $(ls -1 *.go))
if [ -n "${OUTPUT}" ]; then
    echo "${OUTPUT}"
    exit 1
fi

exit 0
