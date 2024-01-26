#!/bin/bash
set -euf -o pipefail

source ./scripts/lib/goqtflags.sh

go install -v honnef.co/go/tools/cmd/staticcheck@latest

cd tailwrap
~/go/bin/staticcheck .
