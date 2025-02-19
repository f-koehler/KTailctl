{
  devenv,
  pkgs,
  lib,
  inputs,
  ...
}:
devenv.lib.mkShell {
  inherit pkgs;
  inherit inputs;
  modules = [
    {
      env.KTAILCTL_WRAPPER_GO_EXECUTABLE = "${lib.getExe' pkgs.go_1_24 "go"}";
      packages = [
        pkgs.go_1_24
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
          package = pkgs.go_1_24;
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
          types = [ "c++" ];
          entry = "clazy-standalone -p build/compile_commands.json";
        };
        cppcheck = {
          enable = true;
          types = [ "c++" ];
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
          types = [ "qml" ];
          entry = "${pkgs.kdePackages.qtdeclarative}/bin/qmlformat -i -n --objects-spacing --functions-spacing";
        };
        qmllint = {
          enable = false;
          types = [ "qml" ];
          entry = "${pkgs.kdePackages.qtdeclarative}/bin/qmllint";
        };

        # Go
        gofmt.enable = true;
        gofumpt = {
          enable = true;
          entry = "${lib.getExe pkgs.gofumpt} -l -w";
          types = [ "go" ];
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
          types = [ "go" ];
        };

        # CMake
        cmake-format = {
          enable = true;
          types = [ "cmake" ];
          entry = "${pkgs.cmake-format}/bin/cmake-format -c .cmake.yaml -i";
        };
        cmake-lint = {
          enable = true;
          types = [ "cmake" ];
          entry = "${pkgs.cmake-format}/bin/cmake-lint -c .cmake.yaml";
        };

        # nix
        nixfmt-rfc-style.enable = true;
        deadnix.enable = true;
        flake-checker.enable = true;
        nil.enable = true;
        statix.enable = true;
      };
    }
  ];
}
