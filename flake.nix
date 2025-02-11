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

  outputs = {
    self,
    nixpkgs,
    devenv,
    systems,
    ...
  } @ inputs: let
    lastModifiedDate = self.lastModifiedDate or self.lastModified or "19700101";
    version = builtins.substring 0 8 lastModifiedDate;
    forEachSystem = nixpkgs.lib.genAttrs (import systems);
    nixpkgsFor = forEachSystem (
      system:
        import nixpkgs {
          inherit system;
          overlays = [self.overlay];
        }
    );
  in {
    packages = forEachSystem (system: {
      devenv-up = self.devShells.${system}.default.config.procfileScript;
      devenv-test = self.devShells.${system}.default.config.test;
      inherit (nixpkgsFor.${system}) ktailctl;
    });
    defaultPackage = forEachSystem (system: self.packages.${system}.ktailctl);

    nixosModules.ktailctl = {pkgs, ...}: {
      nixpkgs.overlays = [self.overlay];
      environment.systemPackages = [pkgs.ktailctl];
    };

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
              vendorHash = "sha256-h/05K/JeJxJbV+G7HUxHZaolLsvGhW66A2ZofRFaafo=";
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

    devShells = forEachSystem (
      system: let
        pkgs = nixpkgs.legacyPackages.${system};
        inherit (nixpkgs) lib;
        go = pkgs.go_1_23;
      in {
        default = devenv.lib.mkShell {
          inherit inputs pkgs;
          modules = [
            {
              env.KTAILCTL_WRAPPER_GO_EXECUTABLE = "${go}/bin/go";
              # https://devenv.sh/reference/options/
              packages = [
                go
                pkgs.gcc
                pkgs.extra-cmake-modules
                pkgs.git
                pkgs.appstream
                pkgs.kdePackages.kdbusaddons
                pkgs.kdePackages.kwindowsystem
                pkgs.kdePackages.kconfig
                pkgs.kdePackages.kcoreaddons
                pkgs.kdePackages.kguiaddons
                pkgs.kdePackages.ki18n
                pkgs.kdePackages.kirigami
                pkgs.kdePackages.kirigami-addons
                pkgs.kdePackages.knotifications
                pkgs.kdePackages.qtbase
                pkgs.kdePackages.qtdeclarative
                pkgs.kdePackages.qtsvg
                pkgs.nlohmann_json
                pkgs.cmake-format
                pkgs.cmake-lint
                pkgs.python3Full
                pkgs.flatpak-builder

                pkgs.libsForQt5.plasma-sdk # for cuttlefish icon browser, not yet updated for Plasma 6
                pkgs.libsForQt5.kirigami-gallery # version in Plasma 6 currently broken in nixpkgs
                pkgs.gdb
                pkgs.lldb
                pkgs.heaptrack # for memory-leak checking

                # C++ linting
                pkgs.flawfinder
                pkgs.cppcheck
                pkgs.clazy

                # Go linting
                pkgs.gosec
                pkgs.go-critic
                pkgs.gofumpt
                pkgs.go-tools
                pkgs.gotools
              ];

              languages = {
                c.enable = true;
                cplusplus.enable = true;
                go = {
                  enable = true;
                  package = go;
                };
              };

              pre-commit.hooks = {
                check-added-large-files.enable = true;
                check-executables-have-shebangs.enable = true;
                check-merge-conflicts.enable = true;
                check-symlinks.enable = true;
                prettier.enable = true;
                # end-of-line-fixer.enable = true;
                # trim-trailing-whitespace = true;
                # end-of-file-fixer = true;

                # C++
                clang-format = {
                  enable = true;
                  types_or = [
                    "c++"
                    "c"
                  ];
                };
                clang-tidy = {
                  enable = false;
                  types_or = [
                    "c++"
                    "c"
                  ];
                  entry = "clang-tidy -p build --fix";
                };
                clazy = {
                  enable = false;
                  types = ["c++"];
                  entry = "clazy-standalone -p build/compile_commands.json";
                };
                cppcheck = {
                  enable = true;
                  types = ["c++"];
                  entry = ''
                    ${lib.getExe pkgs.cppcheck} \
                        --check-level=exhaustive \
                        --enable=performance \
                        --enable=portability \
                        --enable=style \
                        --enable=warning \
                        --library=qt \
                        --error-exitcode=1 \
                        --inline-suppr \
                        --suppress=unusedStructMember
                  '';
                };
                flawfinder = {
                  enable = true;
                  pass_filenames = false;
                  entry = "${lib.getExe pkgs.flawfinder} --error-level=0 ./src";
                };

                # QML
                qmlformat = {
                  enable = true;
                  types = ["qml"];
                  entry = "${pkgs.kdePackages.qtdeclarative}/bin/qmlformat -i -n --objects-spacing --functions-spacing";
                };
                qmllint = {
                  enable = false;
                  types = ["qml"];
                  entry = "${pkgs.kdePackages.qtdeclarative}/bin/qmllint";
                };

                # Go
                gofmt.enable = true;
                gofumpt = {
                  enable = true;
                  entry = "${lib.getExe pkgs.gofumpt} -l -w";
                  types = ["go"];
                };
                gosec = {
                  enable = false;
                  pass_filenames = false;
                  entry = "${lib.getExe pkgs.gosec} src/wrapper";
                };
                go-critic = {
                  enable = true;
                  pass_filenames = false;
                  entry = "${lib.getExe pkgs.go-critic} check src/wrapper";
                };
                gostaticcheck = {
                  enable = true;
                  pass_filenames = false;
                  entry = "${pkgs.go-tools}/bin/staticcheck ./src/wrapper";
                };
                goimports = {
                  enable = true;
                  entry = "${pkgs.gotools}/bin/goimports -w";
                  types = ["go"];
                };

                # CMake
                cmake-format = {
                  enable = true;
                  types = ["cmake"];
                  entry = "${pkgs.cmake-format}/bin/cmake-format -c .cmake.yaml -i";
                };
                cmake-lint = {
                  enable = true;
                  types = ["cmake"];
                  entry = "${pkgs.cmake-format}/bin/cmake-lint -c .cmake.yaml";
                };

                # nix
                alejandra.enable = true;
                deadnix.enable = true;
                flake-checker.enable = true;
                nil.enable = true;
                statix.enable = true;
              };
            }
          ];
        };
      }
    );
  };
}
