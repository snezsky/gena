# Build the all target as ghcr.io/snezsky/gena-ci:latest with docs/ as context.
ARG ANDROID_NDK_VERSION=27.2.12479018

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


FROM base AS android

ARG ANDROID_NDK_VERSION
ARG ANDROID_PLATFORM=36
ARG ANDROID_BUILD_TOOLS=36.0.0
ARG ANDROID_CMDLINE_TOOLS=13114758

# the JDK is only needed by sdkmanager here, Qt images also use it to package APKs
RUN apt-get update && apt-get install -y --no-install-recommends \
        curl \
        openjdk-21-jdk-headless \
        unzip \
    && rm -rf /var/lib/apt/lists/*
ENV JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64
ENV ANDROID_SDK_ROOT=/opt/android-sdk
ENV ANDROID_NDK_ROOT=/opt/android-sdk/ndk/${ANDROID_NDK_VERSION}

RUN curl -fsSL -o /tmp/cmdline-tools.zip https://dl.google.com/android/repository/commandlinetools-linux-${ANDROID_CMDLINE_TOOLS}_latest.zip \
    && mkdir -p ${ANDROID_SDK_ROOT}/cmdline-tools \
    && unzip -q /tmp/cmdline-tools.zip -d ${ANDROID_SDK_ROOT}/cmdline-tools \
    && mv ${ANDROID_SDK_ROOT}/cmdline-tools/cmdline-tools ${ANDROID_SDK_ROOT}/cmdline-tools/latest \
    && rm /tmp/cmdline-tools.zip \
    && yes | ${ANDROID_SDK_ROOT}/cmdline-tools/latest/bin/sdkmanager --licenses > /dev/null \
    && ${ANDROID_SDK_ROOT}/cmdline-tools/latest/bin/sdkmanager --install \
        "platform-tools" \
        "platforms;android-${ANDROID_PLATFORM}" \
        "build-tools;${ANDROID_BUILD_TOOLS}" \
        "ndk;${ANDROID_NDK_VERSION}"


FROM qt-quick AS qt-android

ARG ANDROID_NDK_VERSION

RUN apt-get update && apt-get install -y --no-install-recommends \
        openjdk-21-jdk-headless \
    && rm -rf /var/lib/apt/lists/*
ENV JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64

COPY --from=android /opt/android-sdk /opt/android-sdk
ENV ANDROID_SDK_ROOT=/opt/android-sdk
ENV ANDROID_NDK_ROOT=/opt/android-sdk/ndk/${ANDROID_NDK_VERSION}

# the android kit cross-compiles, its host tools (moc, qmlimportscanner...) come from the desktop Qt
RUN python3 -m venv /tmp/aqt \
    && /tmp/aqt/bin/pip install --no-cache-dir aqtinstall \
    && /tmp/aqt/bin/aqt install-qt all_os android ${QT_VERSION} android_arm64_v8a --outputdir /opt/Qt --archives qtbase qtdeclarative \
    && rm -rf /tmp/aqt
ENV QT_ANDROID_DIR=/opt/Qt/${QT_VERSION}/android_arm64_v8a
ENV QT_HOST_PATH=/opt/Qt/${QT_VERSION}/gcc_64


# Complete CI toolchain: build with docs/ as the Docker build context.
FROM android AS all

ARG QT_VERSION=6.11.3
ENV QT_VERSION=${QT_VERSION}

RUN apt-get update && apt-get install -y --no-install-recommends \
        libasound2t64 libbluetooth3 libdbus-1-3 libdrm2 libegl-dev \
        libfontconfig1 libfreetype6 libgbm1 libgl-dev libglib2.0-0t64 \
        libgssapi-krb5-2 libnspr4 libnss3 libpulse0 libsasl2-2 libspeechd2 \
        libwayland-client0 libwayland-cursor0 libwayland-egl1 \
        libx11-xcb1 libxcb-cursor0 libxcb-icccm4 libxcb-image0 \
        libxcb-keysyms1 libxcb-randr0 libxcb-render-util0 libxcb-shape0 \
        libxcb-xinerama0 libxcb-xkb1 libxcomposite1 libxdamage1 \
        libxkbcommon-x11-0 libxkbcommon0 libxkbfile1 libxrandr2 \
        libxshmfence1 libxtst6 \
    && rm -rf /var/lib/apt/lists/*

COPY install_full_qt.py /tmp/install_full_qt.py
RUN python3 -m venv /tmp/aqt \
    && /tmp/aqt/bin/pip install --no-cache-dir aqtinstall \
    && /tmp/aqt/bin/python /tmp/install_full_qt.py desktop \
    && rm -rf /tmp/aqt
RUN python3 -m venv /tmp/aqt \
    && /tmp/aqt/bin/pip install --no-cache-dir aqtinstall \
    && /tmp/aqt/bin/python /tmp/install_full_qt.py android \
    && rm -rf /tmp/aqt
RUN python3 -m venv /tmp/aqt \
    && /tmp/aqt/bin/pip install --no-cache-dir aqtinstall \
    && /tmp/aqt/bin/python /tmp/install_full_qt.py ifw \
    && rm -rf /tmp/aqt /tmp/install_full_qt.py \
    && rm -f /aqtinstall.log \
    && rm -rf /opt/Qt/Docs /opt/Qt/Examples /opt/Qt/Tools/QtInstallerFramework/4.7/doc

ENV QT_HOST_PATH=/opt/Qt/${QT_VERSION}/gcc_64
ENV QT_ANDROID_DIR=/opt/Qt/${QT_VERSION}/android_arm64_v8a
ENV CMAKE_PREFIX_PATH=/opt/Qt/${QT_VERSION}/gcc_64
ENV QTIFWDIR=/opt/Qt/Tools/QtInstallerFramework/4.7
ENV LANG=C.UTF-8
ENV QT_QPA_PLATFORM=offscreen
ENV PATH="/opt/Qt/${QT_VERSION}/gcc_64/bin:/opt/Qt/Tools/QtInstallerFramework/4.7/bin:/opt/android-sdk/cmdline-tools/latest/bin:/opt/android-sdk/platform-tools:${PATH}"

# Runtime integrations used by WebEngine, desktop themes, printing and SQL plugins.
RUN apt-get update && apt-get install -y --no-install-recommends \
        libxfixes3 libgtk-3-0t64 libcups2t64 libodbc2 libpq5 libfbclient2 \
        libwayland-server0 libvulkan-dev \
    && rm -rf /var/lib/apt/lists/*
