{
  description = "Development environment for KTailctl";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs =
    { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];
      forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f nixpkgs.legacyPackages.${system});
    in
    {
      devShells = forAllSystems (pkgs: {
        default =
          let
            kf = pkgs.kdePackages;
            qtPath = sub: pkgs.lib.makeSearchPath "lib/qt-6/${sub}" qtDeps;
            qtDeps = [
              kf.qtbase
              kf.qtdeclarative
              kf.qtsvg
              kf.qtwayland
              kf.breeze-icons
              kf.kconfig
              kf.kcoreaddons
              kf.kdbusaddons
              kf.kguiaddons
              kf.ki18n
              kf.kiconthemes
              kf.kirigami.unwrapped
              kf.knotifications
              kf.kwindowsystem
              kf.kirigami-addons
              kf.qqc2-desktop-style
            ];
          in
          pkgs.mkShell {
            name = "ktailctl-dev";

            nativeBuildInputs = [
              pkgs.cmake
              pkgs.ninja
              pkgs.pkg-config
              pkgs.go
              pkgs.git
              kf.extra-cmake-modules
              kf.wrapQtAppsHook
            ];

            buildInputs = qtDeps;

            packages = [
              pkgs.clang-tools
              pkgs.gdb
              pkgs.golangci-lint
              pkgs.pre-commit
              kf.qtdeclarative
            ];

            GOTOOLCHAIN = "auto";

            shellHook = ''
              unset CMAKE_TOOLCHAIN_FILE

              export QML2_IMPORT_PATH="${qtPath "qml"}"
              export QT_PLUGIN_PATH="${qtPath "plugins"}"
              export XDG_DATA_DIRS="${pkgs.lib.makeSearchPath "share" qtDeps}''${XDG_DATA_DIRS:+:$XDG_DATA_DIRS}"
            '';
          };
      });

      formatter = forAllSystems (pkgs: pkgs.nixfmt-rfc-style);
    };
}
