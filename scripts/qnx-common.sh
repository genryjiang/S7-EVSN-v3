#!/usr/bin/env bash

QNX_REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
QNX_COMPOSE_FILE="${QNX_REPO_ROOT}/docker/compose.yml"
QNX_ENV_FILE="${QNX_REPO_ROOT}/docker/.env"
QNX_BUILDER_SERVICE="${QNX_BUILDER_SERVICE:-dev-env-builder}"
QNX_COMPOSE_PROJECT="${QNX_COMPOSE_PROJECT:-s7-evsn-qnx}"

qnx_repo_root() {
  printf '%s\n' "${QNX_REPO_ROOT}"
}

qnx_compose() {
  if [[ -f "${QNX_ENV_FILE}" ]]; then
    docker compose \
      --project-name "${QNX_COMPOSE_PROJECT}" \
      --env-file "${QNX_ENV_FILE}" \
      -f "${QNX_COMPOSE_FILE}" \
      "$@"
    return
  fi

  docker compose \
    --project-name "${QNX_COMPOSE_PROJECT}" \
    -f "${QNX_COMPOSE_FILE}" \
    "$@"
}

qnx_require_docker() {
  command -v docker >/dev/null 2>&1 || {
    printf 'Docker CLI is required.\n' >&2
    return 1
  }
  docker info >/dev/null 2>&1 || {
    printf 'Docker daemon is not reachable.\n' >&2
    return 1
  }
}

qnx_require_env_file() {
  if [[ ! -f "${QNX_ENV_FILE}" ]]; then
    printf 'Missing %s. Copy docker/.env.example to docker/.env and set DEV_ENV_BUILDER_IMAGE.\n' "${QNX_ENV_FILE}" >&2
    return 1
  fi
}

qnx_ensure_builder() {
  local builder_id
  builder_id="$(qnx_compose ps -q "${QNX_BUILDER_SERVICE}" 2>/dev/null || true)"
  if [[ -n "${builder_id}" ]]; then
    return 0
  fi

  qnx_compose up -d "${QNX_BUILDER_SERVICE}"
}
