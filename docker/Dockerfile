# syntax=docker/dockerfile:1
FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
ARG USER_NAME=developer
ARG GROUP_NAME=developer
ARG USER_ID=1000
ARG GROUP_ID=1000
ARG QNX_SDP_VERSION=8.0
ARG QNX_MOSQUITTO_REF=35aa903edcceedc2c8010c4c47c7a316a89c9725
ARG TI_PSDK_RTOS_DIR=ti-processor-sdk-rtos-j784s4-evm-11_02_00_06
ARG TI_PSDK_RTOS_ARCHIVE=ti-processor-sdk-rtos-j784s4-evm-11_02_00_06.tar.gz
ARG TI_PSDK_QNX_ARCHIVE=ti-processor-sdk-qnx_800_j784s4_11_02_00_04.tar.gz
ARG TI_QNX_SDP_VERSION=800
ARG TI_QNX_BSP_ARCHIVE=BSP_ti-j784s4-evm_be-800_SVN1019295_JBN342.zip

LABEL description="Sunswift canonical licensed AMD64 QNX build environment"

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
      unzip \
      valgrind \
      wget \
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
    && tar -xzf "/tmp/artifacts/${TI_PSDK_RTOS_ARCHIVE}" -C /opt --no-same-owner --no-same-permissions \
    && tar -xzf "/tmp/artifacts/${TI_PSDK_QNX_ARCHIVE}" -C "/opt/${TI_PSDK_RTOS_DIR}" --no-same-owner --no-same-permissions \
    && mkdir -p "/home/${USER_NAME}/qnx800/bsp" \
    && cp "/tmp/artifacts/${TI_QNX_BSP_ARCHIVE}" "/home/${USER_NAME}/qnx800/bsp/${TI_QNX_BSP_ARCHIVE}" \
    && find "/home/${USER_NAME}/.qnx" -mindepth 1 -print -quit | grep -q . \
    && test -f "/home/${USER_NAME}/qnx800/bsp/${TI_QNX_BSP_ARCHIVE}" \
    && test -f "/opt/${TI_PSDK_RTOS_DIR}/sdk_builder/scripts/setup_psdk_rtos.sh" \
    && test -f "/opt/${TI_PSDK_RTOS_DIR}/psdkqa/psdk_qnx_setup.sh" \
    && test -d "/opt/${TI_PSDK_RTOS_DIR}/psdkqa/qnx" \
    && test -f "/opt/${TI_PSDK_RTOS_DIR}/psdkqa/qnx/qnx_tools_path.mak" \
    && chown -R "${USER_NAME}:${GROUP_NAME}" "/home/${USER_NAME}" "/opt/${TI_PSDK_RTOS_DIR}"

RUN find "/home/${USER_NAME}/rti_connext_dds-7.3.1/lib" \
      -mindepth 1 -maxdepth 1 -type d -printf '%f\n' \
      | grep -Eq 'QNX|QOS' \
    || { echo "RTI archive must contain a QNX target library architecture." >&2; exit 1; }

RUN java -version \
    && test -x "/home/${USER_NAME}/rti_connext_dds-7.3.1/bin/rtiddsgen" \
    && "/home/${USER_NAME}/rti_connext_dds-7.3.1/bin/rtiddsgen" -help >/dev/null

ENV QNX_SDP_VERSION=${QNX_SDP_VERSION}
ENV HOME=/home/${USER_NAME}
ENV USER=${USER_NAME}
ENV QNX_HOST=/home/${USER_NAME}/qnx800/host/linux/x86_64
ENV QNX_TARGET=/home/${USER_NAME}/qnx800/target/qnx
ENV QNX_CONFIGURATION=/home/${USER_NAME}/.qnx
ENV NDDSHOME=/home/${USER_NAME}/rti_connext_dds-7.3.1
ENV RTI_HOME=/home/${USER_NAME}/rti_connext_dds-7.3.1
ENV PSDK_RTOS_PATH=/opt/${TI_PSDK_RTOS_DIR}
ENV PSDK_QNX_PATH=/opt/${TI_PSDK_RTOS_DIR}/psdkqa
ENV PDK_PATH=/opt/${TI_PSDK_RTOS_DIR}/pdk_j784s4_11_02_00_15
ENV TI_QNX_SDP_VERSION=${TI_QNX_SDP_VERSION}
ENV PATH=/home/${USER_NAME}/qnx800/host/linux/x86_64/usr/bin:/home/${USER_NAME}/qnx800/host/linux/x86_64/usr/sbin:/home/${USER_NAME}/rti_connext_dds-7.3.1/bin:${PATH}

USER ${USER_NAME}
WORKDIR /opt/${TI_PSDK_RTOS_DIR}
RUN ./sdk_builder/scripts/setup_psdk_rtos.sh --skip_sudo

WORKDIR /opt/${TI_PSDK_RTOS_DIR}/psdkqa
RUN git init \
    && git config user.name "Docker Builder" \
    && git config user.email "docker-builder@example.invalid" \
    && git commit --allow-empty -m "PSDK QNX setup baseline" \
    && QNX_SDP_VERSION="${TI_QNX_SDP_VERSION}" ./psdk_qnx_setup.sh \
    && rm -rf .git \
    && test -f "${PSDK_QNX_PATH}/psdk_qnx_setup.done" \
    && test -d "${PSDK_QNX_PATH}/pdk" \
    && { test -d "${PSDK_QNX_PATH}/bsp" || test -d "${PSDK_QNX_PATH}/qnx/bsp"; }

WORKDIR /workspace/project
CMD ["sleep", "infinity"]
