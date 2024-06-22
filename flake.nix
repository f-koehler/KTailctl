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
              pname = "ktailctl_wrapper";
              inherit src version;
              modRoot = "src/wrapper";
              vendorHash = "sha256-fAxSh0em+T5+49tx+iz1kQNVikMyut3jzqFuW6Gi9nA=";
            })
            .goModules;

          nativeBuildInputs = [
            cmake
            extra-cmake-modules
            git
            go
            kdePackages.wrapQtAppsHook
          ];
          buildInputs = [
            appstream
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
            cargo
            rustc
            rustfmt
            rustPackages.clippy
            corrosion
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

    devShells = forAllSystems (system: {
      package = nixpkgsFor.${system}.stdenv.mkDerivation {
        name = "ktailctl-devShell-${system}";
        buildInputs = with nixpkgsFor.${system}; [
          appstream
          cmake
          extra-cmake-modules
          git
          go
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
          cargo
          rustc
          rustfmt
          rustPackages.clippy
          corrosion
        ];
      };
    });
  };
}
