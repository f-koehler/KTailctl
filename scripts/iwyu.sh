#!/bin/bash
set -euf -o pipefail

BUILD_DIR="build"
DRY_RUN=true
VERBOSE=false

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
	--verbose | -v)
		VERBOSE=true
		shift
		;;
	*)
		break
		;;
	esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Find iwyu_tool.py and fix_includes.py - they may live outside PATH on some distros
# (e.g. Ubuntu 22.04 places them under /usr/share/doc/iwyu/ instead of /usr/bin/)
_find_tool() {
	local name="$1"
	if command -v "${name}" >/dev/null 2>&1; then
		command -v "${name}"
		return
	fi
	for dir in \
		/usr/share/include-what-you-use \
		/usr/share/doc/iwyu \
		/usr/lib/include-what-you-use; do
		if [ -x "${dir}/${name}" ]; then
			echo "${dir}/${name}"
			return
		fi
	done
	echo "Error: ${name} not found" >&2
	exit 1
}

IWYU_TOOL="$(_find_tool iwyu_tool.py)"
FIX_INCLUDES="$(_find_tool fix_includes.py)"

# Find the Qt IWYU mapping files (path differs between distros)
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

IWYU_ARGS=(-Xiwyu --no_fwd_decls -Xiwyu --mapping_file="${SCRIPT_DIR}/qt6.imp")
if [ -n "${QT_IMP}" ]; then
	IWYU_ARGS+=(-Xiwyu --mapping_file="${QT_IMP}")
fi

IWYU_OUTPUT=$("${IWYU_TOOL}" -p "${BUILD_DIR}" "${SOURCE_FILES[@]}" -- "${IWYU_ARGS[@]}" 2>&1) || true

FIX_ARGS=(--nosafe_headers)
if $DRY_RUN; then
	FIX_ARGS+=(-n)
fi

if $VERBOSE; then
	printf '%s\n' "${IWYU_OUTPUT}" >&2
fi

printf '%s\n' "${IWYU_OUTPUT}" | "${FIX_INCLUDES}" "${FIX_ARGS[@]}"
