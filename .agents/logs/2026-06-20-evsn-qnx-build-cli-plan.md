# 2026-06-20 EVSN QNX Build CLI Plan

Created an execution plan for an original, QNX-only EVSN CLI.

Inputs loaded:

- `harness-creator` skill and multi-agent coordination reference.
- `qnx-cpp-rtos` skill.
- Current repo QNX build scripts, Docker compose file, and execution-plan
  template.
- Legacy S130 tool behavior via read-only explorer subagent.
- Current repo build-surface recommendations via read-only explorer subagent.

Planning decisions:

- The EVSN CLI should be repo-local and callable as `./scripts/evsn`.
- The CLI should be unlocked and should not require `.sunswift-evsn` or any
  replacement marker file.
- The implementation should be clean-sheet work. Legacy tools can inform
  behavior and risk, but their source should not be copied or mechanically
  rewritten.
- QNX builds should use CMake with Ninja through the existing Docker builder
  backend first.
- Subagents should be used again during implementation for disjoint CLI
  implementation, QNX verification review, and documentation/migration review.

Plan path:

- `.agents/exec_plans/evsn-qnx-build-cli.md`

Implementation start:

- Status moved to `in_progress`.
- QNX online documentation evidence added to the plan:
  - `https://www.qnx.com/developers/docs/index.html`
  - `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
  - `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/q/qcc.html`
  - `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/devel_SELFORCROSS.html`
  - `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/envars.html`
- Implementation worker subagent:
  `019ee10d-b344-7b03-bbb8-b2d14adcab31`.

Verification:

- `./init.sh check` passed on 2026-06-20 for the planning artifacts.
- `git diff --check` passed on 2026-06-20.

Implementation completed:

- Added `./scripts/evsn`.
- Added Python stdlib CLI modules under `tools/evsn_cli/`.
- Added focused unit tests under `tests/evsn_cli/`.
- Updated legacy `scripts/qnx-*` wrappers to delegate to EVSN.
- Updated `README.md` with the preferred EVSN QNX CLI workflow.

Subagent review:

- Implementation worker: `019ee10d-b344-7b03-bbb8-b2d14adcab31`.
- QNX verification reviewer: `019ee114-9916-7a72-a2b8-9226162bbc31`.
- Documentation/migration reviewer: `019ee114-b78a-70d2-80bf-9049b5915e43`.

Reviewer findings fixed:

- `qnx-clean --dry-run` now forwards to EVSN without deleting.
- `qnx-build --dry-run target evsn_node` now handles legacy target syntax even
  when flags appear first.
- Artifact verification now checks only the `file` description and requires
  executable artifacts for `verify`.
- The build script clears stale deploy output before install.
- Compile-command refresh refuses non-QNX CMake caches.
- `doctor` validates `DEV_ENV_BUILDER_IMAGE` in `docker/.env` or the
  environment.

Final local verification:

- `python3 -m py_compile tools/evsn_cli/*.py` passed.
- `python3 -m unittest discover -s tests/evsn_cli` passed, 13 tests.
- `bash -n init.sh scripts/*` passed.
- `./scripts/evsn help` passed.
- `./scripts/evsn doctor --offline` passed.
- `./scripts/evsn build --dry-run --target evsn_node` passed.
- `./scripts/evsn clean --dry-run` passed.
- `./scripts/evsn compile-commands --dry-run` passed.
- `./scripts/evsn verify --dry-run --target evsn_node` passed.
- `./scripts/qnx-build --dry-run target evsn_node` passed.
- `./scripts/qnx-clean --dry-run` passed.
- Forbidden-name scan over `tools`, `tests`, `scripts`, and `README.md`
  returned no matches.
- `./init.sh check` passed.
- `git diff --check` passed.

Blocked live verification:

- `./scripts/evsn build --target evsn_node`, `file out/qnx/bin/evsn_node`, and
  `VERIFY_QNX=1 ./init.sh verify` require `docker/.env` or
  `DEV_ENV_BUILDER_IMAGE` to point at a licensed builder image.
