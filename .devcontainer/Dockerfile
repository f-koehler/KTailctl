FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

RUN apt-get update
RUN export DEBIAN_FRONTEND=noninteractive && apt-get -y install --no-install-recommends qtbase5-dev qtdeclarative5-dev qtquickcontrols2-5-dev libqt5svg5-dev
RUN export DEBIAN_FRONTEND=noninteractive && apt-get -y install --no-install-recommends appstream extra-cmake-modules kirigami2-dev libkf5coreaddons-dev libkf5i18n-dev libkf5config-dev libkf5notifications-dev
RUN export DEBIAN_FRONTEND=noninteractive && apt-get -y install --no-install-recommends cmake

COPY ./install-go.sh /tmp/
RUN chmod +x /tmp/install-go.sh && /tmp/install-go.sh && rm -f /tmp/install-go.sh
