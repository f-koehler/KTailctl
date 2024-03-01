#!/bin/bash
set -euf -o pipefail

apt-add-repository 'deb http://archive.neon.kde.org/user jammy main'
wget -qO - 'http://archive.neon.kde.org/public.key' | apt-key add -
apt update
