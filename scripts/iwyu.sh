#!/bin/bash
set -euf -o pipefail

BUILD_DIR="build"
DRY_RUN=true

while [ $# -gt 0 ]; do
	case "$1" in
	-p)
		BUILD_DIR="$2"
		shift 2
		;;
	--fix)
		DRY_RUN=false
		shift
		;;
	*)
		break
		;;
	esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

for cmd in iwyu_tool.py fix_includes.py; do
	if ! command -v "${cmd}" >/dev/null 2>&1; then
		echo "Error: ${cmd} not found" >&2
		exit 1
	fi
done

# Find the Qt IWYU mapping file (path differs between distros)
QT_IMP=""
for path in \
	/usr/share/include-what-you-use/qt5_11.imp \
	/usr/share/iwyu/qt5_11.imp \
	/usr/local/share/include-what-you-use/qt5_11.imp; do
	if [ -f "${path}" ]; then
		QT_IMP="${path}"
		break
	fi
done

if [ -z "${QT_IMP}" ]; then
	echo "Warning: Qt IWYU mapping file not found, running without Qt mappings" >&2
fi

mapfile -t SOURCE_FILES < <(find "${SOURCE_DIR}/src" -name "*.cpp" -type f | sort)

if [ "${#SOURCE_FILES[@]}" -eq 0 ]; then
	echo "Error: no source files found in ${SOURCE_DIR}/src/" >&2
	exit 1
fi

IWYU_ARGS=(-Xiwyu --no_fwd_decls)
if [ -n "${QT_IMP}" ]; then
	IWYU_ARGS+=(-Xiwyu --mapping_file="${QT_IMP}")
fi

IWYU_OUTPUT=$(iwyu_tool.py -p "${BUILD_DIR}" "${SOURCE_FILES[@]}" -- "${IWYU_ARGS[@]}" 2>&1) || true

FIX_ARGS=(--nosafe_headers)
if $DRY_RUN; then
	FIX_ARGS+=(-n)
fi

printf '%s\n' "${IWYU_OUTPUT}" | fix_includes.py "${FIX_ARGS[@]}"
