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
    overlay = final: _: {
      ktailctl = with final;
        stdenv.mkDerivation rec {
          pname = "ktailctl";
          inherit version;

          src = ./.;

          goDeps =
            (buildGo123Module {
              pname = "ktailctl_wrapper";
              inherit src version;
              modRoot = "src/wrapper";
              vendorHash = "sha256-xMWbCV7YmSQ2ieGJTzmRnkVSldBwkZVZc9Zl4WZ767c=";
            })
            .goModules;

          nativeBuildInputs = [
            cmake
            extra-cmake-modules
            git
            go_1_23
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
