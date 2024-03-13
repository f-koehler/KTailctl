#!/bin/bash
set -euf -o pipefail

git clone https://github.com/KDE/clazy
cd clazy
cmake -DCMAKE_CXX_COMPILER=clang++-17 -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build --target install
sudo cmake --build build --target install
