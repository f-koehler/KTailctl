#!/bin/bash
set -euf -o pipefail
flawfinder --error-level=0 ./src/ ./tailwrap
