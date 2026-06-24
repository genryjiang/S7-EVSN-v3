# 2026-06-20 - evsn-can-abstraction-layer-plan

Work item: `VSN-0019`
Plan: `.agents/exec_plans/evsn-can-abstraction-layer.md`
Status: planned before implementation

## Actions

- Ran startup checks and preserved the existing staged/untracked worktree.
- Created branch `feature/evsn-can-abstraction-layer`.
- Read the harness context files required by `AGENTS.md`.
- Loaded the `qnx-cpp-rtos`, `safety-critical-cpp`, and
  `test-coverage-evidence` skills because this plan affects QNX-facing CAN
  access, deterministic C++ boundaries, and verification evidence.
- Inspected the existing `VSN-0017` CAN gateway plan and current EVSN CAN DDS
  type limitations.
- Inspected local `vcan-drivers` README, `commands.h`, SIL FD-CAN emulation
  test, and `cansend`/`candump` README files.
- Consulted QNX Product Documentation and QNX SDP 8.0 online documentation for
  raw CAN `devctl()` behavior and `devctl()` error handling.
- Created `.agents/exec_plans/evsn-can-abstraction-layer.md`.
- Added durable work item `VSN-0019`.
- Updated `VSN-0017` to depend on the HAL plan.
- Added an agent-context note that target-specific CAN headers must stay
  private to HAL backends.

## Findings

- The best implementation boundary is a new `evsn-runtime/can_hal/` package
  with portable fixed-size frame, result, capabilities, diagnostics, and channel
  APIs.
- Gateway code should link `evsn_can_hal` and never directly include QNX,
  `dev-can-linux`, or TDA4 CAN headers.
- Backend selection should be a CMake configuration decision through
  `EVSN_CAN_BACKEND`, with supported values planned as `fake`, `qnx-devctl`,
  `qnx-dev-can-linux`, and `tda4-fdcan`.
- Linux tests should use a fake backend with deterministic injected RX frames,
  recorded TX frames, and forced error states.
- QNX vcan/SIL testing can use raw `devctl()` operations against
  `dev-can-linux -L` endpoints such as `/dev/can0/rx0` and `/dev/can0/tx0`.
- Official QNX SDP 8.0 raw CAN docs describe an 8-byte `struct can_msg`
  payload. FD payload support above 8 bytes must therefore come from a verified
  backend-specific API, not from generic QNX raw CAN assumptions.
- Local `dev-can-linux` evidence supports SIL 64-byte raw container behavior,
  but not physical TDA4 CAN-FD behavior.
- TDA4 support must remain a configure-failing placeholder until vendor
  headers, libraries, API semantics, license terms, target architecture, and
  bench evidence are supplied.
- This HAL work is intentionally DDS-free, so RTI MCP guidance is not required
  for pure HAL implementation. `VSN-0017` DDS gateway integration remains
  blocked on RTI MCP guidance.

## QNX Evidence

- Product documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- CAN `devctl()` command index:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can.html
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_rx_frame_raw_noblock.html
- `CAN_DEVCTL_TX_FRAME_RAW`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html
- C library `devctl()` reference:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/d/devctl.html

## Blockers

- TDA4 FD-CAN vendor headers, libraries, API contract, license terms, and exact
  target architecture are not available in this checkout.
- `docker/.env` does not define `DEV_ENV_BUILDER_IMAGE`, so live QNX build
  verification cannot run.
- Physical CAN-FD behavior on TDA4 requires target or bench evidence before a
  production deployment can use that backend.

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `./init.sh check` passed, validating 19 work items and links in 100 Markdown
  files.
- `git diff --check` passed.

## Next Action

Implement `evsn-runtime/can_hal/` portable frame validation, result types,
diagnostics, fake backend, and CMake backend selection before adding QNX or
TDA4-specific source.
