#!/bin/bash
set -euf -o pipefail

source ./scripts/lib/goqtflags.sh

go install -v github.com/go-critic/go-critic/cmd/gocritic@latest

cd src/wrapper
~/go/bin/gocritic check .
