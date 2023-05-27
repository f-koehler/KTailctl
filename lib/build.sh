set -euf -o pipefail
mkdir -p src build prefix

THREADS=20

cd src
git clone --depth=1 -b kf5 https://invent.kde.org/frameworks/extra-cmake-modules.git
git clone --depth=1 -b kf5 https://invent.kde.org/frameworks/kcoreaddons.git
git clone --depth=1 -b kf5 https://invent.kde.org/frameworks/kconfig.git
git clone --depth=1 -b kf5 https://invent.kde.org/frameworks/ki18n.git
git clone --depth=1 -b kf5 https://invent.kde.org/frameworks/kirigami.git
cd ..

export CMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@5/lib/cmake:/opt/homebrew/opt/gettext
cmake -S src/extra-cmake-modules -B build/extra-cmake-modules -DCMAKE_INSTALL_PREFIX=$PWD/prefix
cmake --build build/extra-cmake-modules --target install

export CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}:${PWD}}/prefix/share/ECM/cmake

cmake -S src/kconfig -B build/kconfig -DCMAKE_INSTALL_PREFIX=$PWD/prefix
cmake --build build/kconfig --target install --parallel=${THREADS}
cmake -S src/kcoreaddons -B build/kcoreaddons -DCMAKE_INSTALL_PREFIX=$PWD/prefix
cmake --build build/kcoreaddons --target install --parallel=${THREADS}
cmake -S src/ki18n -B build/ki18n -DCMAKE_INSTALL_PREFIX=$PWD/prefix
cmake --build build/ki18n --target install --parallel=${THREADS}
cmake -S src/kirigami -B build/kirigami -DCMAKE_INSTALL_PREFIX=$PWD/prefix
cmake --build build/kirigami --target install --parallel=${THREADS}
