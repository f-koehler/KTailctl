#!/bin/bash
set -euf -o pipefail

cppcheck --version
cppcheck \
    --check-level=exhaustive \
    --enable=performance \
    --enable=portability \
    --enable=style \
    --enable=warning \
    --library=qt \
    src/ tailwrap/*.cpp
