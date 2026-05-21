#!/usr/bin/env bash
set -euf -o pipefail

LLVM_VERSION="${LLVM_VERSION:-22}"

sudo apt-get install -yq \
	"clang-${LLVM_VERSION}" \
	"llvm-${LLVM_VERSION}-dev" \
	"libclang-${LLVM_VERSION}-dev"

git clone --depth 1 --branch "clang_${LLVM_VERSION}" \
	https://github.com/include-what-you-use/include-what-you-use.git /tmp/iwyu

LLVM_CMAKE_DIR=$("llvm-config-${LLVM_VERSION}" --cmakedir)
_CMAKE_PARENT=$(dirname "${LLVM_CMAKE_DIR}")
CLANG_CMAKE_DIR=$(find "${_CMAKE_PARENT}" -name "ClangConfig.cmake" -type f 2>/dev/null | head -1 | xargs -r dirname)

if [[ -z "${CLANG_CMAKE_DIR}" ]]; then
	echo "Error: could not find ClangConfig.cmake under ${_CMAKE_PARENT}" >&2
	ls -la "${_CMAKE_PARENT}/" >&2 || true
	exit 1
fi

cmake -B /tmp/iwyu/build -S /tmp/iwyu \
	-G Ninja \
	-DCMAKE_BUILD_TYPE=Release \
	"-DLLVM_DIR=${LLVM_CMAKE_DIR}" \
	"-DClang_DIR=${CLANG_CMAKE_DIR}"

cmake --build /tmp/iwyu/build --parallel
sudo cmake --install /tmp/iwyu/build
