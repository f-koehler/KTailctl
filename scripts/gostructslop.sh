#!/bin/bash
set -euf -o pipefail

source ./scripts/lib/goqtflags.sh

go install -v github.com/orijtech/structslop/cmd/structslop@latest

cd tailwrap
~/go/bin/structslop -fix -verbose .
