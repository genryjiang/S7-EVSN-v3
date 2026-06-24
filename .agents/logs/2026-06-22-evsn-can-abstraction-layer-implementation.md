# 2026-06-22 - evsn-can-abstraction-layer-implementation

Work item: `VSN-0019`
Plan: `.agents/exec_plans/evsn-can-abstraction-layer.md`
Status: implemented core HAL; still `in_progress` pending QNX/SIL runtime and
TDA4 target evidence

## Actions

- Preserved the existing initial dirty checkout state; the repository still has
  no HEAD commit.
- Used subagent role boundaries from `.agents/subagents/README.md`.
- Researcher subagent `019eeda3-d4c2-7e82-b550-4e0ba3927ce4` performed the
  read-only planning pass and confirmed:
  - QNX online documentation evidence exists in the active plan/log.
  - `dev-can-linux` evidence is SIL-only and not physical CAN-FD evidence.
  - `vcan-drivers/dev-can-linux/commands.h` is GPL and must not leak into
    public EVSN headers or be copied into EVSN source.
  - Backend defaults matter because current build wrappers do not pass
    `EVSN_CAN_BACKEND`.
- Implemented `evsn-runtime/can_hal/`:
  - portable `CanFrame`, `CanResult`, `CanOpenConfig`, `CanCapabilities`,
    `CanDiagnostics`, and `CanChannel` public API
  - centralized standard/extended ID, classic/FD length, DLC, remote-frame,
    FD-flag, and backend-capability validation
  - deterministic fixed-capacity fake backend with injected RX, recorded TX,
    and forced status paths
  - `EVSN_CAN_BACKEND` CMake selector defaulting to `fake` on Linux and
    `qnx-devctl` on QNX
  - QNX raw `devctl()` backend using official `<sys/can_dcmd.h>` only
  - clear configure failures for `qnx-dev-can-linux` and `tda4-fdcan`
  - public-header and source boundary scan for target-header leakage,
    `commands.h`, exceptions, RTTI, heap calls, and unbounded containers
- Reviewer subagent `019eedab-5439-7122-b73a-eca8466293be` found two blocking
  issues, both fixed:
  - classic/remote frames now reject FD flags instead of silently dropping them
  - QNX raw backend no longer advertises timestamp support or synthesizes
    monotonic timestamps from undocumented `struct can_msg` timestamp units

## QNX Evidence

- QNX Product Documentation:
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

Design consequences applied:

- RX uses `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK` and maps `EAGAIN` to `no_frame`.
- TX uses `CAN_DEVCTL_TX_FRAME_RAW` and maps `EAGAIN` to `tx_full`.
- Official raw QNX `struct can_msg` is treated as classic CAN only.
- QNX raw timestamp fields are not exposed as monotonic timestamps until target
  documentation or bench evidence defines the unit and clock.

## Verification

- `./scripts/linux-test` passed all 7 CTest tests:
  - `can_hal_validation_test`
  - `can_hal_fake_backend_test`
  - `can_hal_public_boundary_test`
  - existing EVSN runtime tests
- `./scripts/qnx-build --target evsn_can_hal` passed:
  - configured QNX AArch64 with default `EVSN_CAN_BACKEND=qnx-devctl`
  - found `<sys/can_dcmd.h>`
  - built `evsn-runtime/can_hal/libevsn_can_hal.a`
  - refreshed `out/qnx/compile_commands.json`
  - verified installed QNX executable identity through the existing wrapper
- `bash evsn-runtime/can_hal/test/check_public_boundary.sh evsn-runtime/can_hal`
  passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 37 work items and links in 130 Markdown
  files.

## Remaining Gaps

- QNX/SIL runtime smoke with `dev-can-linux -L 1`, `cansend`, and `candump` has
  not run; do not claim vcan runtime behavior yet.
- TDA4 FD-CAN vendor headers, libraries, driver ABI, license terms, target
  architecture, and bench evidence are still unavailable.
- DDS gateway integration remains in `VSN-0017` and requires RTI MCP guidance
  before DDS topic/QoS/lifecycle changes.

## Touched Files

- `evsn-runtime/can_hal/`
- `.agents/exec_plans/evsn-can-abstraction-layer.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/logs/2026-06-22-evsn-can-abstraction-layer-implementation.md`
