{
  version,

  stdenv,
  lib,

  appstream,
  kdePackages,
  nlohmann_json,
  cmake-format,
  cmake,
  go_1_24,
  git,
  extra-cmake-modules,
  buildGoModule,
}:
stdenv.mkDerivation rec {
  pname = "ktailctl";
  inherit version;

  src = ./.;

  goDeps =
    (buildGoModule {
      pname = "ktailctl_wrapper";
      inherit src version;
      modRoot = "src/wrapper";
      vendorHash = "sha256-o7eH3f+yeRr5CnBIuL2jMtVQaBLVihz2dg5RTF8RvaM=";
    }).goModules;

  nativeBuildInputs = [
    cmake
    extra-cmake-modules
    git
    go_1_24
    kdePackages.wrapQtAppsHook
  ];
  buildInputs = [
    appstream
    kdePackages.kdbusaddons
    kdePackages.kwindowsystem
    kdePackages.kconfig
    kdePackages.kcoreaddons
    kdePackages.kguiaddons
    kdePackages.ki18n
    kdePackages.kirigami
    kdePackages.kirigami-addons
    kdePackages.knotifications
    kdePackages.qtbase
    kdePackages.qtdeclarative
    kdePackages.qtsvg
    nlohmann_json
    cmake-format
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
    description = "A GUI to monitor and manage Tailscale on your Linux desktop";
    homepage = "https://github.com/f-koehler/KTailctl";
    license = licenses.gpl3Only;
    mainProgram = "ktailctl";
    platforms = platforms.all;
  };
}
