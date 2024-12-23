{
  pkgs,
  # lib,
  # config,
  # inputs,
  ...
}: let
  go = pkgs.go_1_23;
in {
  # https://devenv.sh/basics/
  env.GREET = "devenv";

  # https://devenv.sh/packages/
  packages = with pkgs; [
    extra-cmake-modules
    git
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
    cmake-lint
    python3Full
    flatpak-builder

    libsForQt5.plasma-sdk # for cuttlefish icon browser, not yet updated for Plasma 6
    libsForQt5.kirigami-gallery # version in Plasma 6 currently broken in nixpkgs
    gdb
    lldb
    heaptrack # for memory-leak checking

    # C++ linting
    flawfinder
    cppcheck
    clazy

    # Go linting
    gosec
    go-critic
    gofumpt
    go-tools
    gotools
  ];

  # https://devenv.sh/scripts/
  # scripts.hello.exec = "echo hello from $GREET";

  enterShell = ''
    export KTAILCTL_WRAPPER_GO_EXECUTABLE=${go}/bin/go
  '';

  # https://devenv.sh/tests/
  # enterTest = ''
  #   echo "Running tests"
  #   git --version | grep "2.42.0"
  # '';

  # https://devenv.sh/services/
  # services.postgres.enable = true;

  # https://devenv.sh/languages/
  languages = {
    c.enable = true;
    cplusplus.enable = true;
    go = {
      enable = true;
      package = go;
    };
  };

  # https://devenv.sh/pre-commit-hooks/
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
      types_or = ["c++" "c"];
    };
    clang-tidy = {
      enable = false;
      types_or = ["c++" "c"];
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
        cppcheck \
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
      entry = "flawfinder --error-level=0 ./src";
    };

    # QML
    qmlformat = {
      enable = true;
      types = ["qml"];
      entry = "qmlformat -i -n --objects-spacing --functions-spacing";
    };
    qmllint = {
      enable = false;
      types = ["qml"];
      entry = "qmllint";
    };

    # Go
    gofmt.enable = true;
    gofumpt = {
      enable = true;
      entry = "gofumpt -l -w";
      types = ["go"];
    };
    gosec = {
      enable = false;
      pass_filenames = false;
      entry = "gosec src/wrapper";
    };
    go-critic = {
      enable = true;
      pass_filenames = false;
      entry = "gocritic check src/wrapper";
    };
    gostaticcheck = {
      enable = true;
      pass_filenames = false;
      entry = "staticcheck ./src/wrapper";
    };
    goimports = {
      enable = true;
      entry = "goimports -w";
      types = ["go"];
    };

    # CMake
    cmake-format = {
      enable = true;
      types = ["cmake"];
      entry = "cmake-format -c .cmake.yaml -i";
    };
    cmake-lint = {
      enable = true;
      types = ["cmake"];
      entry = "cmake-lint -c .cmake.yaml";
    };

    # nix
    alejandra.enable = true;
    deadnix.enable = true;
    flake-checker.enable = true;
    nil.enable = true;
    statix.enable = true;
  };

  # https://devenv.sh/processes/
  # processes.ping.exec = "ping example.com";

  # See full reference at https://devenv.sh/reference/options/
}
