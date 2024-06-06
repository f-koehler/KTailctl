#!/bin/bash
set -euf -o pipefail

source ./scripts/lib/goqtflags.sh

go install -v github.com/securego/gosec/v2/cmd/gosec@latest

cd src/wrapper
~/go/bin/gosec .
