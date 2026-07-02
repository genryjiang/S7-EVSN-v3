#!/usr/bin/env bash
set -euo pipefail

module_dir="${1:?missing j784_mcan_qnx module directory}"

if grep -R -n -E 'GPL-2\.0|MODULE_LICENSE|net_device|sk_buff|can_rx_offload|m_can_classdev|dev-can-linux|qnxCAN1000|SJA1000' \
    "${module_dir}/include" "${module_dir}/src"; then
  printf 'J784 MCAN support package contains copied GPL/Linux/qnxCAN1000 boundary markers.\n' >&2
  exit 1
fi
