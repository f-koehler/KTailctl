{
  pkgs,
  # lib,
  # config,
  # inputs,
  ...
}: {
  # https://devenv.sh/basics/
  env.GREET = "devenv";

  # https://devenv.sh/packages/
  packages = with pkgs; [
    extra-cmake-modules
    git
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
  ];

  # https://devenv.sh/scripts/
  # scripts.hello.exec = "echo hello from $GREET";

  enterShell = ''
    git --version
  '';

  # https://devenv.sh/tests/
  # enterTest = ''
  #   echo "Running tests"
  #   git --version | grep "2.42.0"
  # '';

  # https://devenv.sh/services/
  # services.postgres.enable = true;

  # https://devenv.sh/languages/
  languages.c.enable = true;
  languages.cplusplus.enable = true;
  languages.go.enable = true;

  # https://devenv.sh/pre-commit-hooks/
  pre-commit.hooks = {
    check-added-large-files.enable = true;
    check-executables-have-shebangs.enable = true;
    check-merge-conflicts.enable = true;
    check-symlinks.enable = true;
    cmake-format.enable = true;
    prettier.enable = true;
    # end-of-line-fixer.enable = true;
    # trim-trailing-whitespace = true;
    # end-of-file-fixer = true;

    # C++
    clang-format.enable = true;

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
