#!/bin/bash
# Build KTailctl from scratch on macOS using KDE Craft (https://community.kde.org/Craft).
#
# Assumes Craft is already bootstrapped (see the "macOS via KDE Craft" section of the README)
# and that you've sourced its environment in this shell, e.g.:
#   source ~/CraftRoot/craft/craftenv.sh
# This script does NOT source craftenv.sh itself: craftenv.sh treats KDEROOT already being set
# as "environment already configured" and skips exporting things like MACOSX_DEPLOYMENT_TARGET,
# so re-sourcing it here (after checking KDEROOT below) would silently produce a half-set-up
# environment. Instead it just relies on Craft's environment already being exported by your
# shell, and calls the `craft` executable directly.
set -euf -o pipefail

if [[ "$(uname -s)" != "Darwin" ]]; then
	echo "error: this script only supports macOS" >&2
	exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(dirname "${SCRIPT_DIR}")"
BUILD_DIR="${REPO_ROOT}/build-macos"
BUILD_TYPE="RelWithDebInfo"
JOBS="$(sysctl -n hw.ncpu)"
CLEAN=0

usage() {
	cat <<-EOF
		Usage: $(basename "$0") [options]

		Options:
		  --clean          Remove ${BUILD_DIR} before configuring
		  --debug          Configure a Debug build instead of RelWithDebInfo
		  --jobs N         Parallel build jobs (default: ${JOBS})
		  -h, --help       Show this help
	EOF
}

while [[ $# -gt 0 ]]; do
	case "$1" in
	--clean)
		CLEAN=1
		shift
		;;
	--debug)
		BUILD_TYPE="Debug"
		shift
		;;
	--jobs)
		JOBS="$2"
		shift 2
		;;
	-h | --help)
		usage
		exit 0
		;;
	*)
		echo "error: unknown option: $1" >&2
		usage >&2
		exit 1
		;;
	esac
done

if [[ -z "${KDEROOT:-}" ]]; then
	cat >&2 <<-'EOF'
		error: KDEROOT is not set.

		This script needs a working KDE Craft installation with its environment sourced in this
		shell. Bootstrap one first if you haven't, e.g.:
		  curl https://raw.githubusercontent.com/KDE/craft/master/setup/CraftBootstrap.py -o setup.py
		  python3 setup.py --prefix ~/CraftRoot

		then source Craft's environment before running this script:
		  source ~/CraftRoot/craft/craftenv.sh
	EOF
	exit 1
fi

CRAFT_BIN="$(command -v craft || true)"
if [[ -z "${CRAFT_BIN}" ]]; then
	CRAFT_BIN="${KDEROOT}/craft/bin/craft"
fi
if [[ ! -x "${CRAFT_BIN}" ]]; then
	echo "error: can't find the 'craft' executable (looked on PATH and at ${KDEROOT}/craft/bin/craft)" >&2
	exit 1
fi

# The KF6/Qt6 modules KTailctl needs, on top of what qtbase/ECM already pull in. Mostly served
# straight from Craft's binary cache, so this is quick even on a clean CraftRoot.
CRAFT_PACKAGES=(
	kirigami
	kirigami-addons
	qqc2-desktop-style
	kconfig
	kcoreaddons
	kdbusaddons
	kguiaddons
	ki18n
	kiconthemes
	knotifications
	kwindowsystem
	breeze-icons
	libs/qt6/qtsvg
	libs/qt6/qtdeclarative
)

echo "==> Installing/updating Craft packages"
"${CRAFT_BIN}" "${CRAFT_PACKAGES[@]}"

CMAKE_ARGS=(
	-G Ninja
	-DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
)

if ! command -v go >/dev/null 2>&1; then
	echo "==> No 'go' found on PATH, letting CMake fetch it (-DKTAILCTL_FETCH_GO=ON)"
	CMAKE_ARGS+=(-DKTAILCTL_FETCH_GO=ON)
fi

if [[ "${CLEAN}" -eq 1 ]]; then
	echo "==> Removing ${BUILD_DIR}"
	rm -rf "${BUILD_DIR}"
fi

echo "==> Configuring (${BUILD_TYPE})"
cmake -S "${REPO_ROOT}" -B "${BUILD_DIR}" "${CMAKE_ARGS[@]}"

echo "==> Building with ${JOBS} parallel jobs"
cmake --build "${BUILD_DIR}" --parallel "${JOBS}"

echo "==> Done: ${BUILD_DIR}/bin/ktailctl.app"
