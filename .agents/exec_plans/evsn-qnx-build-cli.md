# evsn-qnx-build-cli

Branch: feature/evsn-qnx-build-cli
Owner: main-agent with subagent review
Status: blocked for live QNX verification

## Scope

Create a clean-sheet, repo-local EVSN build CLI for this repository.

- Primary command: `./scripts/evsn`, with help/banner/docs naming the tool
  `EVSN`.
- Build target: QNX only. No Linux build mode, no mixed host/target package
  flags, and no target launcher in this work item.
- Build engine: CMake with the Ninja generator only.
- Default execution backend: existing Docker builder service from
  `docker/compose.yml`, using the QNX AArch64 toolchain and RTI Connext QNX
  libraries already expected by `scripts/qnx-build`.
- Output model: keep QNX install artifacts under `out/qnx`; keep CMake build
  state isolated from Linux build state.
- CLI must be unlocked: no `.sunswift-evsn` marker, no repo authentication
  sentinel, and no global PATH install requirement. It must run from this
  checkout with `./scripts/evsn`.

## Originality Boundary

The existing S130 build tools are a behavioral reference and a defect list, not
a source template.

- Do not copy source lines from S130 build tools.
- Do not mechanically rewrite old files to make copied code look different.
- Preserve only the useful user-facing concepts: build all, build named targets,
  clean, open a QNX shell, refresh compile commands, and run diagnostics.
- Implement new code, naming, tests, command construction, and documentation in
  EVSN terms.
- Keep legacy project names out of new implementation files except in this plan,
  migration notes, or historical logs where they are necessary context.

## Non-Goals

- No Linux build support in EVSN.
- No DDS package scaffolding replacement in this work item.
- No runtime deploy or target launch behavior until target credentials, launch
  manifests, mutation rules, and log collection are specified separately.
- No AWS or Docker image infrastructure changes unless the CLI needs a small
  compatibility adjustment to call the existing builder.
- No marker-file replacement such as `.evsn`; structural repo detection is
  enough.

## Context Loaded

- `harness-creator` skill and multi-agent coordination reference.
- `qnx-cpp-rtos` skill: prefer repository CMake/wrapper flow, keep behavior
  deterministic, and document QNX assumptions.
- `.agents/exec_plans/README.md` plan format.
- Current repo build surface: `CMakeLists.txt`, `cmake/`, `scripts/qnx-*`,
  `scripts/verify`, `docker/compose.yml`, and `docker/.env.example`.
- Legacy behavior inventory from subagent `019ee0fa-7e9e-7f02-8ed0-67fe15bbe0bc`.
- Current integration recommendations from subagent
  `019ee0fa-9219-7290-9639-d76f2110803e`.

## External Guidance

- RTI MCP: not triggered. This work orchestrates QNX builds and does not design,
  create, or materially change an RTI Connext node, DDS type, QoS profile,
  participant, publisher, subscriber, reader, writer, WaitSet/listener path,
  discovery, durability, liveliness, deadline behavior, or generated-code
  integration.
- QNX online docs:
  - QNX Product Documentation:
    `https://www.qnx.com/developers/docs/index.html`
  - QNX Software Development Platform 8.0:
    `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
  - `q++`, `qcc` compile command:
    `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/q/qcc.html`
  - Cross-development:
    `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/devel_SELFORCROSS.html`
  - Commonly Used Environment Variables:
    `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/envars.html`

## Subagent Plan

- Main agent owns synthesis, final integration, and all user-facing decisions.
- Implementation worker owns `tools/evsn_cli/`, `scripts/evsn`, and focused CLI
  tests. The worker must not edit docs or unrelated build files.
- QNX verification reviewer owns test gaps, dry-run coverage, artifact checks,
  and stale-output risks. This reviewer should avoid implementation edits unless
  explicitly assigned.
- Documentation/migration reviewer owns README updates and compatibility notes
  after the CLI behavior is implemented.
- Workers are not alone in the codebase: they must not revert existing edits and
  must coordinate through this plan and the progress log.

## Proposed Architecture

- Add a Bash-compatible `scripts/evsn` wrapper so existing `bash -n scripts/*`
  verification keeps working.
- Put the implementation in a new Python 3 stdlib package such as
  `tools/evsn_cli/`.
- Keep command construction testable by separating parsing, repo discovery,
  backend command planning, subprocess execution, and diagnostics.
- Resolve repo root in this order:
  1. `--repo-root`
  2. `EVSN_REPO_ROOT`
  3. `git rev-parse --show-toplevel`
  4. parent walk from the current directory
- Validate the repo structurally with files such as `CMakeLists.txt`,
  `cmake/qnx.toolchain.cmake`, `docker/compose.yml`, and
  `evsn-runtime/CMakeLists.txt`. Do not require any marker file.
- Default to the Docker backend. A direct host backend is deferred unless the
  QNX and RTI environment contract is added with tests.
- Always configure with `cmake -G Ninja`, `-DCMAKE_TOOLCHAIN_FILE`, QNX build
  type, `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`, and
  `-DBUILD_TESTING=OFF`.
- Keep the current QNX safeguards: x86_64 builder check, QNX AArch64 compiler
  check, RTI QNX library selection, and refusal to accept non-QNX artifacts.
- Write a machine-readable build transcript under `out/qnx`, including repo
  root, command args, selected RTI architecture, build type, targets, and result.

## CLI Contract

Initial commands:

- `./scripts/evsn help`
- `./scripts/evsn doctor [--offline]`
- `./scripts/evsn build [--target <name>]... [--jobs <n>] [--build-type <type>]`
- `./scripts/evsn clean [--build-cache] [--artifacts] --yes`
- `./scripts/evsn shell`
- `./scripts/evsn compile-commands`
- `./scripts/evsn verify [--target <name>]...`

Quality requirements:

- Clear failures with the exact missing tool, env var, file, or builder
  condition.
- Deterministic target ordering and deterministic command rendering.
- Dry-run support for command planning without requiring Docker or QNX licenses.
- Path guards so clean operations cannot delete outside the repo or Docker build
  cache.
- No interactive prompts in CI-oriented commands.
- Compatibility wrappers may remain, but `scripts/qnx-build`,
  `scripts/qnx-clean`, `scripts/qnx-shell`, and
  `scripts/qnx-compile-commands` should eventually delegate to EVSN or be
  documented as legacy entrypoints.

## Steps

- [x] Freeze the CLI contract and command naming in README before implementation.
      Result: this execution plan defines `./scripts/evsn` as the repo-local
      command and `EVSN` as the CLI name.
- [x] Spawn an implementation worker for the CLI package and wrapper.
      Result: implementation worker subagent
      `019ee10d-b344-7b03-bbb8-b2d14adcab31` implemented the CLI/test slice.
- [x] Add parser and root discovery tests that prove the CLI is unlocked and
      marker-free.
- [x] Add Docker backend planning with pure unit tests for generated CMake,
      Ninja, Docker Compose, and compile-command refresh invocations.
- [x] Implement `doctor`, `build`, `clean`, `shell`, `compile-commands`, and
      `verify`.
- [x] Add dry-run output and a build transcript format.
- [x] Add path-guard tests for clean behavior.
- [x] Update compatibility wrappers or docs so there is one preferred EVSN entry
      point.
- [x] Run a forbidden-name scan over new implementation and docs, excluding
      historical plans/logs.
- [x] Use QNX verification reviewer subagent to audit failure paths and artifact
      checks before marking complete.
- [x] Use documentation/migration reviewer subagent to check README and handoff
      text.

## Verification

- [x] Command: `python3 -m py_compile tools/evsn_cli/*.py`
      Result: Passed on 2026-06-20.
- [x] Command: `python3 -m unittest discover -s tests/evsn_cli`
      Result: Passed on 2026-06-20; ran 14 tests.
- [x] Command: `bash -n init.sh scripts/*`
      Result: Passed on 2026-06-20.
- [x] Command: `./scripts/evsn help`
      Result: Passed on 2026-06-20.
- [x] Command: `./scripts/evsn doctor --offline`
      Result: Passed on 2026-06-20.
- [x] Command: `./scripts/evsn build --dry-run --target evsn_node`
      Result: Passed on 2026-06-20.
- [x] Command: `./init.sh check`
      Result: Passed on 2026-06-20 for the final local implementation;
      validated 17 work items and links in 75 Markdown files.
- [x] Command: `./scripts/evsn compile-commands --dry-run`
      Result: Passed on 2026-06-20.
- [x] Command: `./scripts/evsn shell --dry-run`
      Result: Passed on 2026-06-20.
- [x] Command: `./scripts/evsn verify --dry-run --target evsn_node`
      Result: Passed on 2026-06-20.
- [x] Command: `./scripts/qnx-build --dry-run target evsn_node`
      Result: Passed on 2026-06-20; legacy wrapper handles `target` even when
      flags appear first.
- [x] Command: `./scripts/qnx-compile-commands --dry-run`
      Result: Passed on 2026-06-20.
- [x] Command: `./scripts/qnx-shell --dry-run`
      Result: Passed on 2026-06-20.
- [x] Command: `rg -n "S130|Sunswift|SR-Mjolnir|sr_node|SRNode|\\.sunswift" tools scripts/evsn tests README.md || true`
      Result: Passed on 2026-06-20; no matches.
- [x] Command: `git diff --check`
      Result: Passed on 2026-06-20.
- [x] Command: exact-line originality audit comparing EVSN build-tool files
      against S130 tool files
      Result: Passed on 2026-06-20; found 84 exact shared nonblank stripped
      lines, 0 substantive copied-line candidates after filtering boilerplate
      and generic CLI/Python lines, and a longest contiguous shared block of 2
      generic lines.
- [x] Command: no-context subagent review of S130 CLI A versus EVSN CLI B
      Result: Passed on 2026-06-20 by subagent
      `019ee122-cea6-7091-9d1a-57c291620580`; reviewer reported generic
      overlap only, low substantive implementation overlap, and judged EVSN
      stronger for QNX build orchestration.
- [ ] Command: `./scripts/evsn build --target evsn_node`
      Result: Blocked on 2026-06-20 because `docker/.env` is missing; the CLI
      fails early with instructions to copy `docker/.env.example` and set
      `DEV_ENV_BUILDER_IMAGE`.
- [ ] Command: `file out/qnx/bin/evsn_node`
      Result: Pending live QNX build.
- [ ] Command: `VERIFY_QNX=1 ./init.sh verify`
      Result: Pending live QNX build environment.

## Evidence

- Planning log: `.agents/logs/2026-06-20-evsn-qnx-build-cli-plan.md`
- Implementation log:
  `.agents/logs/2026-06-20-evsn-qnx-build-cli-implementation.md`
- Planning hygiene: `git diff --check` passed on 2026-06-20.
- Final local harness verification: `./init.sh check` passed on 2026-06-20
  with 17 work items and links in 75 Markdown files.
- Final whitespace verification: `git diff --check` passed on 2026-06-20.
- Implementation files: `tools/evsn_cli/`, `scripts/evsn`,
  `tests/evsn_cli/test_cli.py`, delegated `scripts/qnx-*` wrappers, and
  `README.md`.
- Subagent legacy inventory:
  `019ee0fa-7e9e-7f02-8ed0-67fe15bbe0bc`
- Subagent repo integration review:
  `019ee0fa-9219-7290-9639-d76f2110803e`
- Implementation worker:
  `019ee10d-b344-7b03-bbb8-b2d14adcab31`
- QNX verification reviewer:
  `019ee114-9916-7a72-a2b8-9226162bbc31`
- Documentation/migration reviewer:
  `019ee114-b78a-70d2-80bf-9049b5915e43`
- No-context originality/quality reviewer:
  `019ee122-cea6-7091-9d1a-57c291620580`
- Reviewer findings fixed:
  - `qnx-clean --dry-run` now forwards to EVSN without deleting.
  - `qnx-build --dry-run target evsn_node` now handles legacy target syntax even
    when flags appear first.
  - Artifact verification now checks the `file` description, not the artifact
    path, and `verify` requires executable artifacts.
  - Build clears stale `out/qnx` deploy output before install.
  - Compile-command refresh refuses non-QNX CMake caches.
  - `doctor` validates `DEV_ENV_BUILDER_IMAGE` in `docker/.env` or the
    environment.
  - RTI QNX architecture discovery now filters for `qnx|nto` instead of the
    suspicious `QNX|QOS` pattern reported by the no-context reviewer.

## Handoff

The local CLI implementation, tests, dry-run planning, docs, and compatibility
wrappers are in place. Remaining acceptance work is live QNX builder
verification after `docker/.env` is configured with a licensed builder image,
plus optional subagent review before marking this work item passing.
