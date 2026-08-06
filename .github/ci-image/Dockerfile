# Pre-baked CI image for KTailctl.
#
# Bundles everything the "git_hooks", "lint", and "native_build" jobs in
# .github/workflows/ci.yml used to set up from scratch on every single run:
# the Kitware and KDE Neon apt repos, all packages from scripts/neon-deps.sh,
# the lint toolchain (clang-tools, clazy, clang-tidy), and cppcheck/iwyu
# built from source. Baking this once removes that (repeated, slow) setup
# from every CI job.
#
# Built and pushed to ghcr.io by .github/workflows/ci-image.yml, on a
# schedule and whenever this file or the scripts it depends on change.

# Pinned separately from the FROM line (rather than e.g. ubuntu:latest) so
# Dependabot can bump it, and so it's a one-line change once KDE Neon
# publishes packages for a newer codename. Can't go past 24.04 (noble) yet:
# archive.neon.kde.org/user has no dists/ for 26.04's "resolute" codename.
ARG UBUNTU_VERSION=24.04
FROM docker.io/library/ubuntu:${UBUNTU_VERSION}

ENV DEBIAN_FRONTEND=noninteractive
# Picked up by scripts/ci/apt-add-llvm-repo.sh, build-cppcheck.sh and
# build-iwyu.sh below (each defaults to the same values if unset).
ENV LLVM_VERSION=22
ENV CPPCHECK_VERSION=2.20.0

# Tools the setup scripts below need: wget/gnupg for the repo signing keys,
# lsb-release for `lsb_release -c -s`, git for cloning cppcheck/iwyu, sudo
# because the scripts are shared with the (non-root) native CI/dev setup.
RUN apt-get update -yq \
    && apt-get install -yq --no-install-recommends \
        ca-certificates \
        git \
        gnupg \
        lsb-release \
        sudo \
        wget \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/ktailctl-src
COPY scripts/ scripts/

# Mirrors the steps that used to run in ci.yml on every job: add the repos,
# install the build/runtime dependencies plus the lint tool packages, then
# build cppcheck and include-what-you-use from source.
RUN ./scripts/ci/apt-add-kitware-repo.sh \
    && ./scripts/ci/apt-add-kde-neon-repo.sh \
    && ./scripts/ci/apt-add-llvm-repo.sh \
    # apt-add-kde-neon-repo.sh pins the whole Neon origin to priority 900,
    # which would otherwise also shadow Kitware's much newer cmake with
    # Neon's own (older) packaged cmake -- too old for this project's
    # cmake_minimum_required(VERSION 3.31). Force cmake specifically back
    # to Kitware.
    && printf 'Package: cmake cmake-data\nPin: origin apt.kitware.com\nPin-Priority: 1001\n' \
        > /etc/apt/preferences.d/kitware-cmake.pref \
    && apt-get update -yq \
    && ./scripts/neon-deps.sh \
    && apt-get install -yq --no-install-recommends \
        "clang-tidy-${LLVM_VERSION}" \
        clang-tools \
        clazy \
        libpcre3-dev \
    && ./scripts/ci/build-cppcheck.sh \
    && ./scripts/ci/build-iwyu.sh \
    && rm -rf /tmp/cppcheck /tmp/iwyu /var/lib/apt/lists/* \
    && cmake --version

# actions/checkout leaves the workspace owned by a different uid than the
# container runs as; git otherwise refuses to touch it ("dubious ownership").
RUN git config --system --add safe.directory '*'

# Nothing at container-runtime needs root -- all package installs above run
# at image-build time -- so run as a non-root user by default. The ubuntu:*
# base image already ships a passwordless "ubuntu" user (uid/gid 1000) for
# exactly this purpose.
USER ubuntu
WORKDIR /home/ubuntu
