{
  description = "Flake for KTailctl";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = {
    self,
    nixpkgs,
  }: let
    lastModifiedDate = self.lastModifiedDate or self.lastModified or "19700101";
    version = builtins.substring 0 8 lastModifiedDate;
    supportedSystems = ["x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin"];
    forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
    nixpkgsFor = forAllSystems (system:
      import nixpkgs {
        inherit system;
        overlays = [self.overlay];
      });
  in {
    # A Nixpkgs overlay.
    overlay = final: prev: {
      ktailctl = with final;
        stdenv.mkDerivation rec {
          pname = "ktailctl";
          inherit version;

          src = ./.;

          goDeps =
            (buildGoModule {
              pname = "tailwrap";
              inherit src version;
              modRoot = "tailwrap";
              vendorHash = "sha256-NBuV3QKvPcmKBMNpUeHtfVMLRTzgf4eG/j8oR/w4Lew=";
            })
            .goModules;

          nativeBuildInputs = [
            cmake
            extra-cmake-modules
            git
            go
            qt5.wrapQtAppsHook
          ];
          buildInputs = [
            libsForQt5.qt5.qtbase
            libsForQt5.qt5.qtquickcontrols2
            libsForQt5.kconfig
            libsForQt5.kcoreaddons
            libsForQt5.kguiaddons
            libsForQt5.ki18n
            libsForQt5.kirigami2
            libsForQt5.knotifications
            libsForQt5.kirigami-addons
            nlohmann_json
          ];
          runtimeDependencies = [
            libsForQt5.kirigami2
            libsForQt5.kirigami-addons
            libsForQt5.qt5.qtquickcontrols2
          ];

          postPatch = ''
            cp -r --reflink=auto ${goDeps} tailwrap/vendor
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
        };
    };

    packages = forAllSystems (system: {
      inherit (nixpkgsFor.${system}) ktailctl;
    });
    defaultPackage = forAllSystems (system: self.packages.${system}.ktailctl);

    nixosModules.ktailctl = {pkgs, ...}: {
      nixpkgs.overlays = [self.overlay];
      environment.systemPackages = [pkgs.ktailctl];
    };
  };
}
