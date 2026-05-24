#!/usr/bin/env bash

set -euf -o pipefail

KEYRING=/usr/share/keyrings/kde-neon.gpg
CODENAME="$(lsb_release -c -s)"
wget -qO - 'https://archive.neon.kde.org/public.key' | gpg --dearmor - | sudo tee "${KEYRING}" >/dev/null
echo "deb [signed-by=${KEYRING}]" "http://archive.neon.kde.org/user ${CODENAME} main" | sudo tee /etc/apt/sources.list.d/neon.list >/dev/null

# Pin KDE Neon packages above the Ubuntu base repo (default priority 500) so
# that Qt packages from Neon always win over the older Ubuntu versions.
sudo tee /etc/apt/preferences.d/kde-neon.pref >/dev/null <<'EOF'
Package: *
Pin: origin archive.neon.kde.org
Pin-Priority: 900
EOF
