#!/bin/bash
set -euf -o pipefail

# Debug symbols for KTailctl and all its transitive runtime dependencies.
# Installs them locally so GDB/LLDB do not have to fetch them on-demand via
# debuginfod (which is very slow).
#
# 'dnf debuginfo-install' automatically enables the *-debuginfo repos and
# installs both -debuginfo and -debugsource packages for each entry.
# --skip-unavailable silently ignores packages that are not present in the
# configured repos (e.g. nvidia / RPM-Fusion-freeworld entries below that are
# only relevant on certain machines).

PACKAGES=(
	# ── Qt 6 ──────────────────────────────────────────────────────────────
	qt6-qtbase
	qt6-qtbase-gui
	qt6-qtdeclarative
	qt6-qtquickcontrols2
	qt6-qtsvg
	qt6-qtpdf
	qt6-qtimageformats

	# ── KDE Frameworks 6 ──────────────────────────────────────────────────
	kf6-karchive
	kf6-kbookmarks
	kf6-kcodecs
	kf6-kcolorscheme
	kf6-kcompletion
	kf6-kconfig
	kf6-kconfigwidgets
	kf6-kcoreaddons
	kf6-kcrash
	kf6-kdbusaddons
	kf6-kglobalaccel
	kf6-kguiaddons
	kf6-ki18n
	kf6-kiconthemes
	kf6-kimageformats
	kf6-kirigami2
	kf6-kirigami-addons
	kf6-kio-core-libs
	kf6-kio-file-widgets
	kf6-kio-gui
	kf6-kio-widgets-libs
	kf6-kitemviews
	kf6-kjobwidgets
	kf6-knotifications
	kf6-kservice
	kf6-ksolid
	kf6-kstatusnotifieritem
	kf6-kwidgetsaddons
	kf6-kwindowsystem
	kf6-kxmlgui
	kf6-breeze-icons
	kf6-frameworkintegration-libs
	kf6-qqc2-desktop-style
	kf6-sonnet
	kf6-solid

	# ── Plasma ────────────────────────────────────────────────────────────
	plasma-breeze-qt6
	plasma-integration

	# ── OpenGL / GPU ──────────────────────────────────────────────────────
	egl-gbm
	egl-wayland
	egl-wayland2
	egl-x11
	libdrm
	libglvnd
	libglvnd-egl
	libglvnd-glx
	libglvnd-opengl
	libpciaccess
	libxshmfence
	mesa-dri-drivers
	mesa-libEGL
	mesa-libgbm
	# NVIDIA (requires RPM Fusion — skipped automatically if not installed):
	nvidia-driver-libs
	libnvidia-gpucomp

	# ── Wayland ───────────────────────────────────────────────────────────
	libwayland-client
	libwayland-cursor
	libwayland-egl
	libwayland-server

	# ── X11 / XCB ─────────────────────────────────────────────────────────
	libX11
	libX11-xcb
	libXau
	libXcursor
	libXext
	libXfixes
	libXrender
	libxcb
	libxkbcommon
	xcb-util-keysyms

	# ── C / C++ runtime & system libs ─────────────────────────────────────
	glibc
	libgcc
	libstdc++
	libgomp
	libxcrypt
	libffi
	libselinux
	libacl
	libattr
	libcom_err
	keyutils-libs
	pcre2
	pcre2-utf16
	zlib-ng-compat
	xz-libs
	bzip2-libs
	libzstd
	libbrotli
	expat

	# ── systemd / util-linux ──────────────────────────────────────────────
	systemd-libs
	libmount
	libblkid

	# ── D-Bus / GLib ──────────────────────────────────────────────────────
	dbus-libs
	glib2
	libgpg-error
	libgcrypt

	# ── Networking ────────────────────────────────────────────────────────
	openssl-libs
	krb5-libs
	libcurl
	libproxy
	libpsl
	libidn2
	libunistring
	openldap
	libssh
	cyrus-sasl-lib
	libfido2
	libcbor
	libnghttp2
	libnghttp3
	ngtcp2
	ngtcp2-crypto-ossl
	libevent

	# ── Text / fonts ──────────────────────────────────────────────────────
	fontconfig
	freetype
	harfbuzz
	graphite2
	libicu
	fribidi
	libthai
	libdatrie
	pango
	cairo
	cairo-gobject
	pixman

	# ── Images ────────────────────────────────────────────────────────────
	gdk-pixbuf2
	librsvg2
	libpng
	libjpeg-turbo
	libwebp
	libtiff
	libdeflate
	liblerc
	jbigkit-libs
	jasper-libs
	libmng
	lcms2
	openjpeg
	libavif
	libyuv
	libdav1d
	rav1e-libs
	svt-av1-libs
	libaom
	libvmaf
	openexr-libs
	imath
	libheif
	libopenjph
	openh264
	libde265
	LibRaw
	libjxl
	highway
	# RPM Fusion freeworld:
	libheif-freeworld

	# ── Video codecs ──────────────────────────────────────────────────────
	libvpx
	libaribcaption
	aribb24
	opencore-amr
	snappy
	zvbi
	codec2
	# RPM Fusion freeworld:
	libavcodec-freeworld
	libavutil-free
	libswresample-free
	x264-libs
	x265-libs
	xvidcore

	# ── Audio ─────────────────────────────────────────────────────────────
	libvorbis
	libogg
	libcanberra
	soxr
	opus
	speex
	libtheora
	liblc3
	lame-libs
	openapv-libs
	twolame-libs
	vo-amrwbenc
	vvenc-libs
	gsm
	ilbc
	lpcnetfreedv

	# ── GPU / shader / video acceleration ─────────────────────────────────
	llvm-libs
	spirv-tools-libs
	libshaderc
	libva
	libvpl
	libvdpau
	lm_sensors-libs
	numactl-libs
	elfutils-libelf
	libedit
	ncurses-libs

	# ── Misc ──────────────────────────────────────────────────────────────
	double-conversion
	libb2
	libxml2
	duktape
	libtdb
	libtool-ltdl
	libimobiledevice
	libimobiledevice-glue
	libplist
	libusbmuxd
	libseccomp
	glycin-libs
)

dnf debuginfo-install -y --skip-unavailable "${PACKAGES[@]}"
