#!/usr/bin/env bash
set -euf -o pipefail

CPPCHECK_VERSION="${CPPCHECK_VERSION:-2.20.0}"

git clone --depth 1 --branch "${CPPCHECK_VERSION}" https://github.com/danmar/cppcheck.git /tmp/cppcheck
cmake -B /tmp/cppcheck/build -S /tmp/cppcheck -G Ninja -DCMAKE_BUILD_TYPE=Release -DHAVE_RULES=ON
cmake --build /tmp/cppcheck/build --parallel
sudo cmake --install /tmp/cppcheck/build
