{
  devenv,
  pkgs,
  lib,
  inputs,
  ktailctl,
  ...
}:
devenv.lib.mkShell {
  inherit pkgs;
  inherit inputs;
  modules = [
    {
      env = {
        KTAILCTL_WRAPPER_GO_EXECUTABLE = "${lib.getExe' pkgs.go_1_24 "go"}";
      };
      enterShell = ''
        export DEFAULT_CA_BUNDLE_PATH="${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";
        export REQUESTS_CA_BUNDLE="${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";
        export SSL_CERT_FILE="${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";
      '';
      packages = ktailctl.buildInputs;

      languages = {
        c.enable = true;
        cplusplus.enable = true;
        # go = {
        #   enable = true;
        #   package = pkgs.go_1_24;
        # };
        python = {
          enable = true;
          uv.enable = true;
        };
      };

      git-hooks.hooks = {
        check-added-large-files.enable = true;
        check-executables-have-shebangs.enable = true;
        check-merge-conflicts.enable = true;
        check-symlinks.enable = true;
        # prettier.enable = true;
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
          entry = "${lib.getExe' pkgs.clang-tools "clang-tidy"} -p build --fix";
        };
        clazy = {
          enable = false;
          types = [ "c++" ];
          entry = "${lib.getExe pkgs.clazy "clazy-standalone"} -p build/compile_commands.json";
        };
        cppcheck = {
          enable = true;
          types = [ "c++" ];
          entry = ''
            ${lib.getExe' pkgs.cppcheck "cppcheck"} \
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
          entry = "${lib.getExe' pkgs.kdePackages.qtdeclarative "qmlformat"} -i -n --objects-spacing --functions-spacing";
        };
        qmllint = {
          enable = false;
          types = [ "qml" ];
          entry = "${lib.getExe' pkgs.kdePackages.qtdeclarative "qmllint"}";
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
          entry = "${lib.getExe' pkgs.go-tools "staticcheck"} ./src/wrapper";
        };
        goimports = {
          enable = true;
          entry = "${lib.getExe' pkgs.gotools "goimports"} -w";
          types = [ "go" ];
        };

        # CMake
        cmake-format = {
          enable = true;
          types = [ "cmake" ];
          entry = "${lib.getExe' pkgs.cmake-format "cmake-format"} -c .cmake.yaml -i";
        };
        cmake-lint = {
          enable = true;
          types = [ "cmake" ];
          entry = "${lib.getExe' pkgs.cmake-format "cmake-lint"} -c .cmake.yaml";
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
