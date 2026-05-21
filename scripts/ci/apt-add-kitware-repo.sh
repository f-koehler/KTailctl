#!/usr/bin/env bash

set -euf -o pipefail

KEYRING=/usr/share/keyrings/kitware-keyring.gpg
CODENAME="$(lsb_release -c -s)"
wget -qO - "https://apt.kitware.com/keys/kitware-archive-latest.asc" | gpg --dearmor - | sudo tee "${KEYRING}" >/dev/null
echo "deb [signed-by=${KEYRING}]" "https://apt.kitware.com/ubuntu/ ${CODENAME} main" | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
