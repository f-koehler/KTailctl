#!/bin/bash
set -euf -o pipefail

source ./scripts/lib/goqtflags.sh

go install -v honnef.co/go/tools/cmd/staticcheck@latest

cd src/wrapper
~/go/bin/staticcheck .
