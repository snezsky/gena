# Dockerfile used for building the CI images at ghcr.io/snezsky/gena-*
FROM ubuntu:26.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        ca-certificates \
        ccache \
        clang \
        clang-format \
        clang-tidy \
        cmake \
        cppcheck \
        g++ \
        gcc \
        gcovr \
        git \
        libclang-rt-dev \
        llvm \
        ninja-build \
        python3 \
        python3-venv \
    && rm -rf /var/lib/apt/lists/*

RUN python3 -m venv /opt/codechecker \
    && /opt/codechecker/bin/pip install --no-cache-dir codechecker==6.29.1
ENV PATH="/opt/codechecker/bin:${PATH}"

RUN command -v run-clang-tidy \
    || ln -s "$(ls /usr/bin/run-clang-tidy-[0-9]* | grep -v '\.py$' | sort -V | tail -n 1)" /usr/bin/run-clang-tidy


FROM base AS qt

ARG QT_VERSION=6.11.3
ENV QT_VERSION=${QT_VERSION}

# system libraries the official Qt binaries build and run against
RUN apt-get update && apt-get install -y --no-install-recommends \
        libdbus-1-3 \
        libegl-dev \
        libfontconfig1 \
        libfreetype6 \
        libgl-dev \
        libglib2.0-0t64 \
        libxcb-cursor0 \
        libxcb-icccm4 \
        libxcb-image0 \
        libxcb-keysyms1 \
        libxcb-randr0 \
        libxcb-render-util0 \
        libxcb-shape0 \
        libxcb-xinerama0 \
        libxcb-xkb1 \
        libxkbcommon-x11-0 \
        libxkbcommon0 \
    && rm -rf /var/lib/apt/lists/*

# the official Qt build, unlike Ubuntu's packages, can be deployed (bundled) with the application
RUN python3 -m venv /tmp/aqt \
    && /tmp/aqt/bin/pip install --no-cache-dir aqtinstall \
    && /tmp/aqt/bin/aqt install-qt linux desktop ${QT_VERSION} linux_gcc_64 --outputdir /opt/Qt --archives qtbase icu \
    && rm -rf /tmp/aqt
ENV CMAKE_PREFIX_PATH=/opt/Qt/${QT_VERSION}/gcc_64
ENV PATH="/opt/Qt/${QT_VERSION}/gcc_64/bin:${PATH}"
ENV QT_QPA_PLATFORM=offscreen


FROM qt AS qt-quick

RUN python3 -m venv /tmp/aqt \
    && /tmp/aqt/bin/pip install --no-cache-dir aqtinstall \
    && /tmp/aqt/bin/aqt install-qt linux desktop ${QT_VERSION} linux_gcc_64 --outputdir /opt/Qt --archives qtdeclarative \
    && rm -rf /tmp/aqt \
    # static internals of the QML language server and formatter (~950 MB), applications never link them
    && rm /opt/Qt/${QT_VERSION}/gcc_64/lib/libQt6QmlLS.a /opt/Qt/${QT_VERSION}/gcc_64/lib/libQt6QmlDom.a
