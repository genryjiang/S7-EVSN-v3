#!/usr/bin/env bash
set -euo pipefail

module_dir="${1:?missing j784_mcan_qnx module directory}"

if grep -R -n -E '#[[:space:]]*include[[:space:]]*[<"]((sys|ha|hw|linux)/|devctl\.h|fcntl\.h|unistd\.h|commands\.h)' \
    "${module_dir}/include"; then
  printf 'J784 MCAN support headers expose target-specific or GPL helper headers.\n' >&2
  exit 1
fi

if grep -R -n -E 'SocketCAN|qnxCAN1000|GPL|sk_buff|net_device|m_can_platform|dev-can-linux' \
    "${module_dir}/include" "${module_dir}/src"; then
  printf 'J784 MCAN support package contains copied prior-art or Linux driver boundary markers.\n' >&2
  exit 1
fi

if grep -R -n -E 'throw|dynamic_cast|typeid|std::vector|std::string|malloc|calloc|realloc|free' \
	"${module_dir}/include" \
	"${module_dir}/src"; then
  printf 'J784 MCAN host-testable support path uses disallowed exception, RTTI, heap, or unbounded container patterns.\n' >&2
  exit 1
fi
