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

if ! command -v cppcheck >/dev/null 2>&1; then
	echo "Error: cppcheck not found" >&2
	exit 1
fi

exec cppcheck \
	--project="${BUILD_DIR}/compile_commands.json" \
	--file-filter="${SOURCE_DIR}/src/*" \
	--library=qt \
	--enable=warning,style,performance,portability \
	--error-exitcode=1 \
	--inline-suppr \
	--suppress=missingIncludeSystem \
	--suppress=noDestructor \
	--suppress="knownConditionTrueFalse" \
	--suppress="noExplicitConstructor:*${BUILD_DIR}/*" \
	"$@"
