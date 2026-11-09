%global debug_package %{nil}

Name:           ktailctl
Version:        0.22.0
Release:        1%{?dist}
Summary:        GUI to monitor and manage Tailscale on Linux

License:        GPL-3.0-or-later
URL:            https://github.com/f-koehler/KTailctl
Source0:        https://github.com/f-koehler/KTailctl/archive/v%{version}/KTailctl-%{version}.tar.gz

BuildRequires:  cmake >= 3.31
BuildRequires:  extra-cmake-modules >= 6.24
BuildRequires:  pkgconfig
BuildRequires:  qt6-qtbase-devel >= 6.10
BuildRequires:  qt6-qtdeclarative-devel >= 6.10
BuildRequires:  qt6-qtsvg-devel >= 6.10
BuildRequires:  qt6-qtwayland-devel >= 6.10
BuildRequires:  kf6-kconfig-devel >= 6.24
BuildRequires:  kf6-kcoreaddons-devel >= 6.24
BuildRequires:  kf6-kdbusaddons-devel >= 6.24
BuildRequires:  kf6-kguiaddons-devel >= 6.24
BuildRequires:  kf6-ki18n-devel >= 6.24
BuildRequires:  kf6-kirigami-devel >= 6.24
BuildRequires:  kirigami-addons-devel >= 6.24
BuildRequires:  kf6-knotifications-devel >= 6.24
BuildRequires:  kf6-kwindowsystem-devel >= 6.24
BuildRequires:  nlohmann-json-devel
BuildRequires:  golang
BuildRequires:  desktop-file-utils
BuildRequires:  appstream

Requires:       tailscale
Requires:       qt6-qtbase
Requires:       qt6-qtdeclarative
Requires:       kf6-kirigami
Requires:       kf6-kconfig
Requires:       kf6-kcoreaddons
Requires:       kf6-ki18n

%description
A native KDE desktop client for monitoring and managing your Tailscale mesh
network. Built with Kirigami, it provides system tray integration and a
full GUI for controlling your Tailscale network.

%prep
%autosetup -n KTailctl-%{version}

%build
cd src/tailscale/wrapper && go mod vendor && cd ../../..
cmake -B builddir -DCMAKE_INSTALL_PREFIX=/usr -DKTAILCTL_FLATPAK_BUILD=ON
cmake --build builddir

%install
DESTDIR=%{buildroot} cmake --install builddir

%files
%{_bindir}/ktailctl
%{_prefix}/lib/x86_64-linux-gnu/libktailctl_wrapper.a
%{_prefix}/lib/x86_64-linux-gnu/libktailctl_wrapper_logging.so
%{_datadir}/applications/org.fkoehler.KTailctl.desktop
%{_datadir}/metainfo/org.fkoehler.KTailctl.metainfo.xml
%{_datadir}/icons/hicolor/scalable/apps/org.fkoehler.KTailctl.svg
%license LICENSE.txt

%changelog
* Wed Jul 30 2026 Fame <fame@famelinuxpc> - 0.22.0-1
- Initial RPM package release
