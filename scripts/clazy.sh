#!/bin/bash
set -euf -o pipefail

BUILD_DIR="build"
CHECKS="level1"

while [ $# -gt 0 ]; do
	case "$1" in
	-p)
		BUILD_DIR="$2"
		shift 2
		;;
	--checks)
		CHECKS="$2"
		shift 2
		;;
	*)
		break
		;;
	esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

mapfile -t SOURCE_FILES < <(find "${SOURCE_DIR}/src" -name "*.cpp" -type f | sort)

if [ "${#SOURCE_FILES[@]}" -eq 0 ]; then
	echo "Error: no source files found in ${SOURCE_DIR}/src/" >&2
	exit 1
fi

for cmd in clazy-standalone; do
	if command -v "${cmd}" >/dev/null 2>&1; then
		exec "${cmd}" \
			-p "${BUILD_DIR}" \
			--checks="${CHECKS}" \
			-extra-arg=-Werror \
			"$@" \
			"${SOURCE_FILES[@]}"
	fi
done

echo "Error: clazy-standalone not found" >&2
exit 1
