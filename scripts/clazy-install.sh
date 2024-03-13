#!/bin/bash
set -euf -o pipefail

git clone https://github.com/KDE/clazy
cd clazy
cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
sudo cmake --build build --target install
