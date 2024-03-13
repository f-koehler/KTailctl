#!/bin/bash
cmake \
    -DCMAKE_CXX_COMPILER=clazy \
    -DCMAKE_CXX_FLAGS="-Werror -Xclang -plugin-arg-clazy -Xclang level1,no-fully-qualified-moc-types" \
    -B build-clazy
cmake --build build-clazy