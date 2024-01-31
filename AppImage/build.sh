#!/bin/bash
mkdir -p build-AppImage AppDir
PATH="/usr/local/go/bin:${PATH}" cmake -B build-AppImage -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/usr ..
cmake --build build-AppImage
make -C build-AppImage install DESTDIR=${PWD}/AppDir
/usr/local/bin/appimage-builder
