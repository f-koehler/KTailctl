#!/bin/bash
set -euf -o pipefail

go install -v github.com/securego/gosec/v2/cmd/gosec@latest

cd tailwrap
~/go/bin/gosec .
