{
  version,

  stdenv,
  lib,

  appstream,
  buildGo124Module,
  cmake,
  cmake-format,
  extra-cmake-modules,
  git,
  go_1_24,
  kdePackages,
  nlohmann_json,
}:
let
  src = ./.;

  goDeps =
    (buildGo124Module {
      pname = "ktailctl_wrapper";
      inherit src version;
      modRoot = "src/wrapper";
      vendorHash = "sha256-Cjm9CsbmsuPPLBmKF1HLGrUoxwqYFvuOASoXzf8UB3g=";
    }).goModules;
in
stdenv.mkDerivation {
  pname = "ktailctl";

  inherit version;
  inherit goDeps;
  inherit src;

  nativeBuildInputs = [
    cmake
    extra-cmake-modules
    git
    go_1_24
    kdePackages.wrapQtAppsHook
  ];
  buildInputs = [
    appstream
    cmake-format
    kdePackages.kconfig
    kdePackages.kcoreaddons
    kdePackages.kdbusaddons
    kdePackages.kguiaddons
    kdePackages.ki18n
    kdePackages.kirigami
    kdePackages.kirigami-addons
    kdePackages.knotifications
    kdePackages.kwindowsystem
    kdePackages.qqc2-desktop-style
    kdePackages.qtbase
    kdePackages.qtdeclarative
    kdePackages.qtsvg
    kdePackages.qtwayland
    nlohmann_json
  ];

  postPatch = ''
    cp -r --reflink=auto ${goDeps} src/wrapper/vendor
  '';

  preBuild = ''
    export HOME=$TMPDIR
  '';

  cmakeFlags = [
    "-DKTAILCTL_FLATPAK_BUILD=ON"
  ];
  meta = with lib; {
    description = "GUI to monitor and manage Tailscale on your Linux desktop";
    homepage = "https://github.com/f-koehler/KTailctl";
    license = licenses.gpl3Only;
    mainProgram = "ktailctl";
    platforms = platforms.unix;
  };
}
