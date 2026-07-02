#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MODE="${1:-check}"

failures=0

note() {
  printf '%s\n' "$*"
}

fail() {
  printf 'ERROR: %s\n' "$*" >&2
  failures=$((failures + 1))
}

require_file() {
  local path="$1"
  if [ ! -f "$ROOT_DIR/$path" ]; then
    fail "missing required file: $path"
  else
    note "ok file: $path"
  fi
}

require_dir() {
  local path="$1"
  if [ ! -d "$ROOT_DIR/$path" ]; then
    fail "missing required directory: $path"
  else
    note "ok dir: $path"
  fi
}

check_json() {
  local path="$1"
  if command -v jq >/dev/null 2>&1; then
    jq empty "$ROOT_DIR/$path" >/dev/null || fail "invalid JSON: $path"
  elif command -v python3 >/dev/null 2>&1; then
    python3 -m json.tool "$ROOT_DIR/$path" >/dev/null || fail "invalid JSON: $path"
  else
    note "skip JSON validation for $path: jq/python3 not found"
  fi
}

require_text() {
  local path="$1"
  local needle="$2"
  local label="$3"
  if grep -Fq "$needle" "$ROOT_DIR/$path"; then
    note "ok guard: $label"
  else
    fail "missing harness guard in $path: $label"
  fi
}

check_shell_syntax() {
  local script
  while IFS= read -r script; do
    [ -n "$script" ] || continue
    bash -n "$ROOT_DIR/$script" || fail "shell syntax failed: $script"
    note "ok shell: $script"
  done < <(cd "$ROOT_DIR" && find scripts -type f -name '*.sh' -print 2>/dev/null | sort)

  bash -n "$ROOT_DIR/init.sh" || fail "shell syntax failed: init.sh"
  note "ok shell: init.sh"
}

detect_infra_dir() {
  if [ -d "$ROOT_DIR/Dev-Env-Builder" ]; then
    printf '%s\n' "Dev-Env-Builder"
    return
  fi
  if [ -d "$ROOT_DIR/cloud-infra" ]; then
    printf '%s\n' "cloud-infra"
    return
  fi
  printf '%s\n' "Dev-Env-Builder"
}

check_sr_skill_refs() {
  local path
  for path in \
    ".agents/skills/qnx-cpp-rtos/SKILL.md" \
    ".agents/skills/qnx-cpp-performance/SKILL.md" \
    ".agents/skills/safety-critical-cpp/SKILL.md" \
    ".agents/skills/iso26262-readiness/SKILL.md" \
    ".agents/skills/rti-connext-dds/SKILL.md" \
    ".agents/skills/mqtt-middleware/SKILL.md" \
    ".agents/skills/cpp-unit-testing/SKILL.md" \
    ".agents/skills/cpp-integration-testing/SKILL.md" \
    ".agents/skills/middleware-testing/SKILL.md" \
    ".agents/skills/fault-injection-resilience-testing/SKILL.md" \
    ".agents/skills/test-coverage-evidence/SKILL.md" \
    ".agents/skills/qnx-realtime-control-loop/SKILL.md"; do
    require_file "$path"
  done
}

check_external_guidance_guards() {
  require_text "AGENTS.md" "RTI_MCP_GUIDANCE_REQUIRED" "RTI MCP required for RTI Connext node/DDS work"
  require_text "AGENTS.md" "run the RTI Connext MCP for guidance" "RTI MCP must be run before RTI node work"
  require_text "AGENTS.md" "QNX_ONLINE_DOCS_REQUIRED" "online QNX docs required for QNX-facing work"
  require_text "AGENTS.md" "offline references" "QNX online documentation is mandatory"
  require_text ".agents/skills/rti-connext-dds/SKILL.md" "RTI_MCP_GUIDANCE_REQUIRED" "RTI skill carries MCP guard"
  require_text ".agents/skills/qnx-cpp-rtos/SKILL.md" "QNX_ONLINE_DOCS_REQUIRED" "QNX skill carries online-docs guard"
  require_text ".agents/exec_plans/README.md" "External Guidance" "execution plans include external guidance evidence"
  require_text ".agents/subagents/README.md" "RTI_MCP_GUIDANCE_REQUIRED" "subagents carry RTI MCP evidence guard"
  require_text ".agents/subagents/README.md" "QNX_ONLINE_DOCS_REQUIRED" "subagents carry QNX docs evidence guard"
}

run_node_validation() {
  if ! command -v node >/dev/null 2>&1; then
    fail "node is required for EVSN harness validation"
    return
  fi

  node "$ROOT_DIR/.agents/validate-feature-list.mjs" || fail "feature list validation failed"
  node "$ROOT_DIR/.agents/validate-doc-links.mjs" || fail "Markdown link validation failed"
}

run_check() {
  note "== Harness files =="
  require_file "AGENTS.md"
  require_file "init.sh"
  require_file ".agents/README.md"
  require_file ".agents/feature_list.json"
  require_file ".agents/progress.md"
  require_file ".agents/session-handoff.md"
  require_file ".agents/agent-context.md"
  require_file ".agents/exec_plans/README.md"
  require_file ".agents/exec_plans/sr-mjolnir-oai-layout-port.md"
  require_file ".agents/logs/2026-06-20-sr-mjolnir-oai-layout-port.md"
  require_file ".agents/subagents/README.md"
  require_file ".agents/tooling/harness-onboarding.md"
  require_file ".agents/tooling/qnx/ham_smoke.c"
  require_file ".agents/schemas/work-items.schema.json"
  require_file ".agents/validate-feature-list.mjs"
  require_file ".agents/validate-doc-links.mjs"
  require_dir ".agents/skills"
  require_file "docker/README.md"
  require_file "docker/.env.example"
  require_file "docker/compose.yml"
  require_file "docker/Dockerfile"
  require_file "docker/buildspec.yml"
  require_file "CMakeLists.txt"
  require_file "cmake/EVSNNodeHelpers.cmake"
  require_file "cmake/qnx.toolchain.cmake"
  require_file "evsn-runtime/CMakeLists.txt"
  require_file "evsn-runtime/evsn_node/evsn_node.hpp"
  require_file "evsn-runtime/evsn_node/evsn_node.cpp"
  require_file "evsn-runtime/evsn_node/CMakeLists.txt"
  require_file "evsn-runtime/dds_types/std_types/CMakeLists.txt"
  require_file "evsn-runtime/dds_types/custom_types/CMakeLists.txt"
  require_file "scripts/linux-build"
  require_file "scripts/linux-test"
  require_file "scripts/qnx-build"
  require_file "scripts/qnx-common.sh"
  require_file "scripts/verify"
  require_file "scripts/test-no-evsnnode-mqtt"
  local infra_dir
  infra_dir="$(detect_infra_dir)"
  require_file "$infra_dir/README.md"
  require_file "$infra_dir/package.json"
  require_file "$infra_dir/cdk.json"
  require_file "$infra_dir/bin/dev-env-builder.ts"
  require_file "$infra_dir/lib/dev-env-builder-stack.ts"
  require_file "$infra_dir/test/dev-env-builder.test.ts"
  check_sr_skill_refs
  check_json ".agents/feature_list.json"
  check_json ".agents/schemas/work-items.schema.json"
  check_json "$infra_dir/package.json"
  check_json "$infra_dir/cdk.json"
  check_external_guidance_guards

  note "== EVSN harness validation =="
  run_node_validation

  note "== Shell syntax =="
  check_shell_syntax

  note "== Git state =="
  git -C "$ROOT_DIR" status --short || fail "git status failed"

  if [ "$failures" -ne 0 ]; then
    fail "$failures harness check(s) failed"
    exit 1
  fi
  note "Harness check passed."
}

run_quick() {
  run_check
  note "== Tool availability =="
  for tool in git node docker cmake bash python3 jq; do
    if command -v "$tool" >/dev/null 2>&1; then
      note "ok tool: $tool ($(command -v "$tool"))"
    else
      note "missing optional tool: $tool"
    fi
  done
}

run_setup() {
  git -C "$ROOT_DIR" submodule update --init --recursive

  if [ -f "$ROOT_DIR/docker/.env" ] && [ -f "$ROOT_DIR/docker/compose.yml" ]; then
    docker compose --env-file "$ROOT_DIR/docker/.env" -f "$ROOT_DIR/docker/compose.yml" pull dev-env-builder
    docker compose --env-file "$ROOT_DIR/docker/.env" -f "$ROOT_DIR/docker/compose.yml" up -d dev-env-builder
  else
    fail "missing docker/.env or docker/compose.yml for Docker setup flow"
  fi

  if [ -x "$ROOT_DIR/scripts/qnx-sync-headers" ]; then
    "$ROOT_DIR/scripts/qnx-sync-headers"
  else
    note "skip header sync: scripts/qnx-sync-headers not found"
  fi

  if [ "$failures" -ne 0 ]; then
    fail "$failures setup check(s) failed"
    exit 1
  fi
}

run_verify() {
  run_check

  if [ -x "$ROOT_DIR/scripts/verify" ]; then
    note "== Build and test application =="
    "$ROOT_DIR/scripts/verify"
  else
    note "skip application verification: scripts/verify not found"
  fi

  if [ "${VERIFY_QNX:-0}" = "1" ]; then
    if [ -x "$ROOT_DIR/scripts/qnx-build" ]; then
      note "== QNX build =="
      "$ROOT_DIR/scripts/qnx-build"
    else
      fail "VERIFY_QNX=1 requested but scripts/qnx-build not found"
      exit 1
    fi
  fi
}

case "$MODE" in
  check)
    run_check
    ;;
  quick)
    run_quick
    ;;
  setup)
    run_setup
    ;;
  verify)
    run_verify
    ;;
  help|-h|--help)
    cat <<'USAGE'
Usage: ./init.sh [check|quick|setup|verify]

check   Validate OAI-style harness files and EVSN work-item content.
quick   Run check and print availability of common QNX/EVSN tools.
setup   Run the EVSN setup flow when project Docker files exist.
verify  Run check, then application verification when scripts/verify exists.

Set VERIFY_QNX=1 with verify to additionally require scripts/qnx-build.
USAGE
    ;;
  *)
    fail "unknown mode: $MODE"
    exit 2
    ;;
esac
