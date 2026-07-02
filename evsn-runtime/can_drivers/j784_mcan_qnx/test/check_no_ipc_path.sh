#!/usr/bin/env bash
set -euo pipefail

module_dir="${1:?missing j784_mcan_qnx module directory}"

if grep -R -n -E 'RPMessage|remoteproc|tiipc|TISCI_MSG_RM|R5F|r5f|mailbox|virtio|ipc' \
    "${module_dir}/include" "${module_dir}/src"; then
  printf 'J784 MCAN support package contains an IPC or remote-core CAN path.\n' >&2
  exit 1
fi
