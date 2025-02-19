{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    systems.url = "github:nix-systems/default";
    devenv.url = "github:cachix/devenv";
    devenv.inputs.nixpkgs.follows = "nixpkgs";
  };

  nixConfig = {
    extra-trusted-public-keys = "devenv.cachix.org-1:w1cLUi8dv3hnoSPGAuibQv+f9TZLr6cv/Hm9XgU50cw=";
    extra-substituters = "https://devenv.cachix.org";
  };

  outputs =
    {
      self,
      nixpkgs,
      devenv,
      systems,
      ...
    }@inputs:
    let
      lastModifiedDate = self.lastModifiedDate or self.lastModified or "19700101";
      version = builtins.substring 0 8 lastModifiedDate;
      forEachSystem = nixpkgs.lib.genAttrs (import systems);
      nixpkgsFor = forEachSystem (
        system:
        import nixpkgs {
          inherit system;
          overlays = [ self.overlay ];
        }
      );
    in
    {
      packages = forEachSystem (system: {
        devenv-up = self.devShells.${system}.default.config.procfileScript;
        devenv-test = self.devShells.${system}.default.config.test;
        inherit (nixpkgsFor.${system}) ktailctl;
      });
      defaultPackage = forEachSystem (system: self.packages.${system}.ktailctl);

      nixosModules.ktailctl =
        { pkgs, ... }:
        {
          nixpkgs.overlays = [ self.overlay ];
          environment.systemPackages = [ pkgs.ktailctl ];
        };

      overlay = final: _: {
        ktailctl =
          with final;
          stdenv.mkDerivation rec {
            pname = "ktailctl";
            inherit version;

            src = ./.;

            goDeps =
              (buildGo123Module {
                pname = "ktailctl_wrapper";
                inherit src version;
                modRoot = "src/wrapper";
                vendorHash = "sha256-o7eH3f+yeRr5CnBIuL2jMtVQaBLVihz2dg5RTF8RvaM=";
              }).goModules;

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

      devShells = forEachSystem (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          inherit (nixpkgs) lib;
          go = pkgs.go_1_23;
        in
        {
          default = import ./shell.nix {
            inherit devenv;
            inherit pkgs;
            inherit lib;
            inherit go;
            inherit inputs;
          };
        }
      );
    };
}
