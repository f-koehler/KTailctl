#!/bin/bash
set -euf -o pipefail

BUILD_DIR="build"

while [ $# -gt 0 ]; do
	case "$1" in
	-p)
		BUILD_DIR="$2"
		shift 2
		;;
	*)
		break
		;;
	esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

for cmd in run-clang-tidy-22 run-clang-tidy-21; do
	if command -v "${cmd}" >/dev/null 2>&1; then
		exec "${cmd}" -p "${BUILD_DIR}" "$@" "^${SOURCE_DIR}/src/"
	fi
done

echo "Error: no run-clang-tidy binary found" >&2
exit 1
