#!/usr/bin/env bash

set -euf -o pipefail

LLVM_VERSION="${LLVM_VERSION:-22}"
KEYRING=/usr/share/keyrings/llvm-keyring.gpg
CODENAME="$(lsb_release -c -s)"
wget -qO - "https://apt.llvm.org/llvm-snapshot.gpg.key" | gpg --dearmor - | sudo tee "${KEYRING}" >/dev/null
echo "deb [signed-by=${KEYRING}]" "http://apt.llvm.org/${CODENAME}/ llvm-toolchain-${CODENAME}-${LLVM_VERSION} main" | sudo tee /etc/apt/sources.list.d/llvm.list >/dev/null
