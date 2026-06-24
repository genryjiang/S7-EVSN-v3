# 2026-06-20 EVSN QNX Build CLI Implementation

Implemented the repo-local EVSN QNX CLI from
`.agents/exec_plans/evsn-qnx-build-cli.md`.

Files added:

- `scripts/evsn`
- `tools/__init__.py`
- `tools/evsn_cli/__init__.py`
- `tools/evsn_cli/__main__.py`
- `tools/evsn_cli/cli.py`
- `tests/evsn_cli/test_cli.py`

Files updated:

- `scripts/qnx-build`
- `scripts/qnx-clean`
- `scripts/qnx-shell`
- `scripts/qnx-compile-commands`
- `README.md`
- `.gitignore`
- `.agents/exec_plans/evsn-qnx-build-cli.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`

Implementation notes:

- `./scripts/evsn` is the preferred entrypoint.
- The implementation is Python 3 stdlib only and keeps command construction
  testable.
- Repo discovery is structural and marker-free.
- Build, compile-command refresh, shell, clean, and verify commands use the
  existing Docker builder service from `docker/compose.yml`.
- QNX build planning uses CMake with Ninja, the repository QNX toolchain file,
  `BUILD_TESTING=OFF`, AArch64 QNX compiler checks, RTI QNX library checks, and
  artifact refusal checks.
- Legacy `scripts/qnx-*` wrappers delegate to `./scripts/evsn`.

Verification:

- `python3 -m py_compile tools/evsn_cli/*.py` passed.
- `python3 -m unittest discover -s tests/evsn_cli` passed with 14 tests.
- `bash -n init.sh scripts/*` passed.
- `./scripts/evsn help` passed.
- `./scripts/evsn doctor --offline` passed.
- `./scripts/evsn build --dry-run --target evsn_node` passed.
- `./scripts/evsn compile-commands --dry-run` passed.
- `./scripts/evsn shell --dry-run` passed.
- `./scripts/evsn verify --dry-run --target evsn_node` passed.
- `./scripts/qnx-build --dry-run target evsn_node` passed.
- `./scripts/qnx-compile-commands --dry-run` passed.
- `./scripts/qnx-shell --dry-run` passed.
- Forbidden legacy-name scan over new implementation/docs passed with no
  matches.
- `./init.sh check` passed, validating 17 work items and links in 75 Markdown
  files.
- `git diff --check` passed.

Originality and independent review:

- Exact-line audit compared 9 EVSN build-tool files against 7 S130 tool files.
  It found 84 exact shared nonblank stripped lines, all boilerplate or generic
  CLI/Python lines after filtering, 0 substantive copied-line candidates, and a
  longest contiguous shared block of 2 generic lines.
- No-context explorer subagent `019ee122-cea6-7091-9d1a-57c291620580` reviewed
  the S130 and EVSN CLIs from file paths only. It reported generic overlap only,
  low substantive implementation overlap, and judged the EVSN CLI stronger for
  QNX build orchestration because of explicit command structure, deterministic
  target handling, injected-testable entrypoints, dry-run support, QNX artifact
  checks, and non-interactive CI behavior.
- The independent review flagged the RTI architecture filter `QNX|QOS` as
  suspicious. The EVSN CLI now uses the case-insensitive QNX/Neutrino-oriented
  filter `qnx|nto`, and the unit suite covers that regression.

Blocker:

- `./scripts/evsn build --target evsn_node` is blocked because `docker/.env` is
  missing. The CLI fails early and instructs the user to copy
  `docker/.env.example` to `docker/.env` and set `DEV_ENV_BUILDER_IMAGE`.

QNX documentation consulted:

- `https://www.qnx.com/developers/docs/index.html`
- `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
