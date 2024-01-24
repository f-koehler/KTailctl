#!/bin/bash
set -euf -o pipefail

go install -v honnef.co/go/tools/cmd/staticcheck@latest

cd tailwrap
~/go/bin/staticcheck .
