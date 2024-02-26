#!/bin/bash
mkdir -p build-AppImage AppDir
PATH="/usr/local/go/bin:${PATH}" cmake -B build-AppImage -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/usr -DKTAILCTL_APPIMAGE_BUILD=ON ..
cmake --build build-AppImage
make -C build-AppImage install DESTDIR=${PWD}/AppDir
./appimage-builder --skip-tests
