# syntax=docker/dockerfile:1
FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
ARG USER_NAME=developer
ARG GROUP_NAME=developer
ARG USER_ID=1000
ARG GROUP_ID=1000
ARG QNX_SDP_VERSION=8.0
ARG QNX_MOSQUITTO_REF=35aa903edcceedc2c8010c4c47c7a316a89c9725

LABEL description="Generic licensed AMD64 QNX and RTI build environment"

ENV UNDERSTAND_VERSION=5.1
ENV UNDERSTAND_BUILD=1004
ENV STIHOME=/opt/scitools
ENV PATH="/opt/scitools/bin/linux64:${PATH}"

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
      bash \
      build-essential \
      ca-certificates \
      clang-14 \
      clang-format-14 \
      cmake \
      file \
      git \
      iproute2 \
      libglib2.0-0 \
      libgtest-dev \
      libmosquitto-dev \
      libncurses5 \
      libpcsclite-dev \
      mosquitto \
      mosquitto-clients \
      ninja-build \
      openjdk-17-jre-headless \
      pkg-config \
      python3 \
      valgrind \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang-14 100 \
    && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-14 100 \
    && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-14 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-14 100 \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-14 100

RUN cmake -S /usr/src/googletest -B /tmp/googletest-build -G Ninja \
    && cmake --build /tmp/googletest-build --target install \
    && rm -rf /tmp/googletest-build

ADD "https://s3.amazonaws.com/builds.scitools.com/all_builds/b${UNDERSTAND_BUILD}/Understand/Understand-${UNDERSTAND_VERSION}.${UNDERSTAND_BUILD}-Linux-64bit.tgz" /tmp/

RUN tar -xzf "/tmp/Understand-${UNDERSTAND_VERSION}.${UNDERSTAND_BUILD}-Linux-64bit.tgz" -C /opt \
    && rm -f "/tmp/Understand-${UNDERSTAND_VERSION}.${UNDERSTAND_BUILD}-Linux-64bit.tgz" \
    && test -x /opt/scitools/bin/linux64/und \
    && und -version

RUN groupadd --gid "${GROUP_ID}" "${GROUP_NAME}" \
    && useradd --uid "${USER_ID}" --gid "${GROUP_ID}" --create-home --shell /bin/bash "${USER_NAME}"

RUN --mount=type=bind,source=artifacts,target=/tmp/artifacts,ro \
    tar -xzf /tmp/artifacts/qnx800.tar.gz -C "/home/${USER_NAME}" --no-same-owner --no-same-permissions \
    && tar -xzf /tmp/artifacts/rti_connext-7.3.1.tar.gz -C "/home/${USER_NAME}" --no-same-owner --no-same-permissions \
    && mkdir -p "/home/${USER_NAME}/.qnx" \
    && tar -xzf /tmp/artifacts/qnx_license.tar.gz -C "/home/${USER_NAME}/.qnx" --no-same-owner --no-same-permissions \
    && find "/home/${USER_NAME}/.qnx" -mindepth 1 -print -quit | grep -q . \
    && chown -R "${USER_NAME}:${GROUP_NAME}" "/home/${USER_NAME}"

RUN find "/home/${USER_NAME}/rti_connext_dds-7.3.1/lib" \
      -mindepth 1 -maxdepth 1 -type d -printf '%f\n' \
      | grep -Eq 'QNX|QOS' \
    || { echo "RTI archive must contain a QNX target library architecture." >&2; exit 1; }

RUN java -version \
    && test -x "/home/${USER_NAME}/rti_connext_dds-7.3.1/bin/rtiddsgen" \
    && "/home/${USER_NAME}/rti_connext_dds-7.3.1/bin/rtiddsgen" -help >/dev/null

ENV QNX_SDP_VERSION="${QNX_SDP_VERSION}"
ENV QNX_HOST="/home/${USER_NAME}/qnx800/host/linux/x86_64"
ENV QNX_TARGET="/home/${USER_NAME}/qnx800/target/qnx"
ENV QNX_CONFIGURATION="/home/${USER_NAME}/.qnx"
ENV NDDSHOME="/home/${USER_NAME}/rti_connext_dds-7.3.1"
ENV RTI_HOME="/home/${USER_NAME}/rti_connext_dds-7.3.1"
ENV PATH="/home/${USER_NAME}/qnx800/host/linux/x86_64/usr/bin:/home/${USER_NAME}/qnx800/host/linux/x86_64/usr/sbin:/home/${USER_NAME}/rti_connext_dds-7.3.1/bin:${PATH}"

RUN git clone https://github.com/qnx-ports/mosquitto.git /tmp/qnx-mosquitto \
    && cd /tmp/qnx-mosquitto \
    && git checkout "${QNX_MOSQUITTO_REF}" \
    && cmake -S /tmp/qnx-mosquitto -B /tmp/qnx-mosquitto-build -G Ninja \
      -DCMAKE_SYSTEM_NAME=QNX \
      -DCMAKE_SYSTEM_PROCESSOR=aarch64le \
      -DCMAKE_C_COMPILER="${QNX_HOST}/usr/bin/aarch64-unknown-nto-qnx8.0.0-gcc-12.2.0" \
      -DCMAKE_CXX_COMPILER="${QNX_HOST}/usr/bin/aarch64-unknown-nto-qnx8.0.0-g++-12.2.0" \
      -DCMAKE_SYSROOT="${QNX_TARGET}" \
      -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
      -DCMAKE_INSTALL_PREFIX=/opt/qnx-mosquitto/aarch64le \
      -DCMAKE_BUILD_TYPE=Release \
      -DWITH_TLS=OFF \
      -DWITH_TLS_PSK=OFF \
      -DWITH_EC=OFF \
      -DWITH_CLIENTS=OFF \
      -DWITH_BROKER=OFF \
      -DWITH_CJSON=OFF \
      -DWITH_APPS=OFF \
      -DWITH_PLUGINS=OFF \
      -DWITH_SRV=OFF \
      -DWITH_SOCKS=OFF \
      -DWITH_UNIX_SOCKETS=OFF \
      -DWITH_WEBSOCKETS=OFF \
      -DWITH_LIB_CPP=OFF \
      -DDOCUMENTATION=OFF \
    && cmake --build /tmp/qnx-mosquitto-build \
    && cmake --install /tmp/qnx-mosquitto-build \
    && test -f /opt/qnx-mosquitto/aarch64le/include/mosquitto.h \
    && test -f /opt/qnx-mosquitto/aarch64le/include/mqtt_protocol.h \
    && test -e /opt/qnx-mosquitto/aarch64le/lib/libmosquitto.so \
    && tar -C /opt/qnx-mosquitto -czf /opt/qnx-mosquitto-aarch64le.tar.gz aarch64le \
    && rm -rf /tmp/qnx-mosquitto /tmp/qnx-mosquitto-build

WORKDIR /workspace/project
USER developer
CMD ["sleep", "infinity"]
