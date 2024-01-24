#!/bin/bash
set -eu -o pipefail

go install -v github.com/sqs/goreturns@latest

cd tailwrap
OUTPUT=$(~/go/bin/goreturns -l -d -w $(ls -1 *.go))
if [ -n "${OUTPUT}" ]; then
    echo "${OUTPUT}"
    exit 1
fi

exit 0
