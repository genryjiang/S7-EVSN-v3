#!/usr/bin/env bash
set -euo pipefail

module_dir="${1:?missing can_hal module directory}"

if grep -R -n -E '#[[:space:]]*include[[:space:]]*<((sys/can_dcmd|devctl|fcntl|unistd)\\.h|ha/|hw/)|commands\\.h|TDA4|tda4|J784|j784|MCAN|mcan' \
    "${module_dir}/include"; then
  printf 'Public CAN HAL headers expose target-specific headers or names.\n' >&2
  exit 1
fi

if grep -R -n -E 'throw|dynamic_cast|typeid|std::vector|std::string|malloc|calloc|realloc|free' \
    "${module_dir}/include" "${module_dir}/src"; then
  printf 'CAN HAL uses disallowed exception, RTTI, heap, or unbounded container patterns.\n' >&2
  exit 1
fi

if grep -R -n -E 'commands\\.h|EXT_CAN_DEVCTL_SET_LATENCY_LIMIT_MS' "${module_dir}/src"; then
  printf 'CAN HAL source uses dev-can-linux private GPL helper APIs before approval.\n' >&2
  exit 1
fi
