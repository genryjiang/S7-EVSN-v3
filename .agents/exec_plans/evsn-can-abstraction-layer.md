# VSN-0019 - EVSN CAN Abstraction Layer

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: in_progress

## Scope

Design and implement a target-selectable CAN abstraction layer for the EVSN CAN
gateway. The layer must let the gateway build against the currently available
QNX `dev-can-linux` virtual CAN resource-manager path for SIL/cloud testing and
later build against board-specific TDA4 FD-CAN headers and libraries without
leaking target headers into portable gateway code.

The intended implementation is a new package under `evsn-runtime/can_hal/`.
The gateway in `VSN-0017` should depend on the HAL instead of directly
including QNX, `dev-can-linux`, or future TDA4 CAN headers.

The HAL is responsible for:

- Providing one portable, fixed-size internal `CanFrame` model for CAN 2.0 and
  FD-CAN traffic.
- Exposing explicit, non-throwing open, read, write, close, capabilities,
  diagnostics, and optional configuration operations.
- Building a fake backend for Linux unit tests and DDS-free gateway tests.
- Building a QNX raw `devctl()` backend that works with QNX CAN
  resource-manager endpoints such as `/dev/can0/rx0` and `/dev/can0/tx0`.
- Supporting `dev-can-linux -L` vcan endpoints through the QNX raw `devctl()`
  backend for virtual/cloud/SIL verification.
- Reserving a compile-time selected TDA4 FD-CAN backend that is enabled only
  when the vendor header and library locations are supplied.
- Keeping all target-specific headers and libraries private to backend source
  files and CMake targets.

## Non-Goals

- Do not change RTI Connext DDS types, QoS, participants, readers, writers, or
  topic names in this work item. DDS integration remains under `VSN-0017`.
- Do not decode DBC signals or move DBC pack/unpack into the HAL.
- Do not implement the TDA4 backend until the actual board driver headers,
  libraries, ABI, supported FD flags, endpoint model, and license terms are
  available.
- Do not copy GPL helper implementations from `vcan-drivers/dev-can-linux`.
  Prefer official QNX raw `devctl()` calls in EVSN-owned source; any optional
  use of `commands.h` requires project/legal approval and must remain private
  to a QNX-only backend.
- Do not claim physical CAN-FD support from `dev-can-linux` vcan evidence.
  Treat vcan as SIL/raw-frame evidence only.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/exec_plans/evsn-can-gateway.md`
- `.agents/skills/qnx-cpp-rtos/SKILL.md`
- `.agents/skills/safety-critical-cpp/SKILL.md`
- `.agents/skills/test-coverage-evidence/SKILL.md`
- `.agents/skills/qnx-cpp-rtos/references/qnx-neutrino-rtos.yaml`
- `.agents/skills/qnx-cpp-rtos/references/qnx-advanced.yaml`
- `.agents/skills/safety-critical-cpp/references/real-time-coding-rules.md`
- `.agents/skills/safety-critical-cpp/references/memory-management-rules.md`
- `.agents/skills/test-coverage-evidence/references/test-automation-rules.md`
- `evsn-runtime/CMakeLists.txt`
- `evsn-runtime/dds_types/std_types/evsn_fdcan/evsn_fdcan.idl`
- `evsn-runtime/dds_types/std_types/evsn_can20/evsn_can20.idl`
- `vcan-drivers/README.md`
- `vcan-drivers/dev-can-linux/commands.h`
- `vcan-drivers/tests/sil/fd-can-emulation-standby-success-case.mjs`
- `vcan-drivers/tools/cansend/README.md`
- `vcan-drivers/tools/candump/README.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix for bus/frame compatibility requirements above the HAL boundary

## External Guidance

- RTI MCP:
  - Not required for this HAL plan because this work item deliberately excludes
    DDS entities, DDS types, QoS, topic naming, generated-code integration, and
    node lifecycle changes.
  - Design consequence: pure HAL types, backends, conversion tests, and CMake
    backend selection may proceed without RTI MCP guidance. Integrating the HAL
    into the DDS CAN gateway remains governed by `VSN-0017` and its RTI MCP
    blocker.

- QNX online docs:
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

QNX design consequences:

- A QNX backend should use `<sys/can_dcmd.h>` and raw-frame `devctl()` commands
  where the selected driver supports raw mode.
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK` is the preferred receive primitive for the
  first backend because it returns `EAGAIN` when no frame is queued, preserving
  bounded shutdown and loop timing.
- `CAN_DEVCTL_TX_FRAME_RAW` can return `EAGAIN` when the transmit queue is full;
  the HAL must report that as `CanStatus::tx_full` without retrying forever.
- Standard QNX `struct can_msg` exposes an 8-byte `dat` field in official SDP
  8.0 documentation. FD payloads above 8 bytes must not be assumed portable
  through the official raw `devctl()` ABI.
- `dev-can-linux` local evidence defines `CAN_MSG_DATA_MAX_CANFD` and accepts
  64-byte raw containers for SIL, but that is repository-local driver behavior,
  not a generic QNX CAN contract.

## Proposed Architecture

The HAL is bus-agnostic. It must not encode the roadmap hardware-equivalence
matrix directly, but its fake and QNX/SIL tests must be capable of exercising
all classic CAN 2.0 frames required by that matrix, including CAN0 receive
fanout frames and the WS22 `0x501` transmit frame.

### Package Layout

Planned files:

- `evsn-runtime/can_hal/CMakeLists.txt`
- `evsn-runtime/can_hal/README.md`
- `evsn-runtime/can_hal/include/evsn/can_hal/can_frame.hpp`
- `evsn-runtime/can_hal/include/evsn/can_hal/can_result.hpp`
- `evsn-runtime/can_hal/include/evsn/can_hal/can_channel.hpp`
- `evsn-runtime/can_hal/src/common/*.cpp`
- `evsn-runtime/can_hal/src/fake/*.cpp`
- `evsn-runtime/can_hal/src/qnx/*.cpp`
- `evsn-runtime/can_hal/src/tda4/*.cpp`
- `evsn-runtime/can_hal/test/*.cpp`

Public headers should include only standard C++ headers and EVSN HAL headers.
Backend source files are the only place where `<sys/can_dcmd.h>`,
`vcan-drivers/dev-can-linux/commands.h`, or future TDA4 vendor headers may
appear.

### Portable Frame Model

Use one internal frame type for every backend:

- `uint32_t id`
- `CanIdFormat` with `standard_11_bit` and `extended_29_bit`
- `CanFrameFormat` with `classic` and `fd`
- `CanFrameKind` with `data` and `remote`
- `uint8_t dlc`
- `uint8_t length`
- `std::array<uint8_t, 64> payload`
- `CanFdFlags` for `brs`, `esi`, and `fdf` where a backend can preserve them
- optional monotonic timestamp field with an explicit validity flag

Validation rules:

- Standard IDs must be `<= 0x7ff`.
- Extended IDs must be `<= 0x1fffffff`.
- Classic frames accept payload lengths `0..8`.
- FD frames accept payload lengths `0..64`.
- Remote frames must not carry FD payloads or unsupported metadata.
- DLC and payload length must be validated by a central helper before any
  backend writes a frame.

### Public API Shape

Use explicit result objects, no exceptions, and no target-native types:

- `CanOpenConfig`
  - fixed-size RX path and TX path storage
  - requested bus mode: `classic`, `fd`, or `auto_detect`
  - optional latency limit
  - optional bitrate/arbitration/data timing request, disabled by default
- `CanCapabilities`
  - classic frame support
  - FD frame support
  - extended-ID support
  - remote-frame support
  - BRS/ESI/FDF flag support
  - timestamp support
  - latency-limit support
  - runtime bitrate-change support
- `CanDiagnostics`
  - opens, closes, RX frames, TX frames
  - no-frame polls, TX full events
  - invalid frame rejects
  - unsupported feature rejects
  - native error code and operation of last backend failure
- `CanStatus`
  - `ok`, `no_frame`, `tx_full`, `invalid_argument`, `invalid_frame`,
    `unsupported`, `not_open`, `permission_denied`, `io_error`,
    `configuration_error`
- `CanChannel`
  - `open(const CanOpenConfig&) noexcept`
  - `read(CanFrame&) noexcept`
  - `write(const CanFrame&) noexcept`
  - `capabilities() noexcept`
  - `diagnostics() noexcept`
  - `close() noexcept`

Avoid dynamic allocation after `open()`. Keep any backend dispatch either
compile-time selected through CMake or a fixed function table initialized once
during startup. Do not require RTTI or exceptions.

### Backend Selection

Add one CMake cache selector:

- `EVSN_CAN_BACKEND=fake`
- `EVSN_CAN_BACKEND=qnx-devctl`
- `EVSN_CAN_BACKEND=qnx-dev-can-linux`
- `EVSN_CAN_BACKEND=tda4-fdcan`

Expected defaults:

- Linux host/test builds default to `fake`.
- QNX builds default to `qnx-devctl` unless the deployment explicitly selects
  `qnx-dev-can-linux` or `tda4-fdcan`.
- `qnx-dev-can-linux` is for vcan/SIL only unless target evidence approves it
  for a physical driver.
- `tda4-fdcan` must fail CMake configuration unless all required variables are
  supplied, for example:
  - `EVSN_TDA4_CAN_INCLUDE_DIR`
  - `EVSN_TDA4_CAN_LIBRARY`
  - `EVSN_TDA4_CAN_DRIVER_NAME`
  - `EVSN_TDA4_CAN_EXPECTED_ARCH`

CMake target shape:

- `evsn_can_hal_core`: portable frame, validation, diagnostics, fake hooks.
- `evsn_can_hal_backend_fake`: host-only fake backend for unit tests.
- `evsn_can_hal_backend_qnx_devctl`: QNX backend using official raw
  `devctl()` commands.
- `evsn_can_hal_backend_qnx_dev_can_linux`: optional QNX/SIL backend for
  `dev-can-linux` extensions, private include paths only.
- `evsn_can_hal_backend_tda4_fdcan`: future board backend, private vendor
  include paths and libraries only.
- `evsn_can_hal`: the single public target linked by `can_gateway`, privately
  linking exactly one backend.

### Backend Responsibilities

Fake backend:

- Runs on Linux without QNX headers or RTI.
- Provides deterministic injected RX frames and recorded TX frames.
- Lets tests force `no_frame`, `tx_full`, `io_error`, invalid capability, and
  close behavior.

QNX `devctl()` backend:

- Opens configured RX and TX endpoints with checked POSIX calls.
- Uses `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK` for bounded receive polling.
- Uses `CAN_DEVCTL_TX_FRAME_RAW` for transmit.
- Maps `EAGAIN` on receive to `no_frame`.
- Maps `EAGAIN` on transmit to `tx_full`.
- Maps unsupported raw mode or mailbox mismatch errors to explicit statuses.
- Converts QNX standard MID bits 18..28 and extended MID bits 0..28 to the
  portable ID model.
- Treats official QNX raw `struct can_msg` as classic CAN only unless a
  selected driver API proves wider FD payload support.

QNX `dev-can-linux` backend:

- Uses the same `/dev/canN/rxM` and `/dev/canN/txM` endpoint model.
- Supports `dev-can-linux -L <n>` vcan loopback for cloud/SIL testing.
- May expose the driver-specific latency-limit extension only behind an
  optional compile flag and after license approval.
- Must record that its 64-byte FD container support is local SIL behavior, not
  physical TDA4 evidence.

TDA4 FD-CAN backend:

- Waits for board-specific headers and libraries.
- Must document whether the vendor API is QNX resource-manager based, direct
  library calls, or another IPC model.
- Must map hardware FD flags, data-phase bitrate, controller state, bus-off,
  error-passive, TX queue pressure, and timestamp behavior into HAL
  capabilities and diagnostics.
- Must fail startup if the selected bus requires FD-CAN behavior that the
  backend cannot preserve.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on fake-backend unit tests alone.

Minimum evidence:

- smoke tests for backend selection, configuration validation, and bounded
  open/close behavior
- unit tests for classic and FD lengths, DLC conversion, standard and extended
  IDs, RTR handling, unsupported capability errors, backend error statuses, and
  malformed frames
- integration tests with fake CAN and QNX/SIL `dev-can-linux` adapters where
  target access exists
- regression tests proving public headers do not expose QNX, `dev-can-linux`,
  or TDA4 vendor headers and that no steady-state HAL path requires dynamic
  allocation
- exact commands, backend selection, platform/target details, logs/traces,
  exit status, and known gaps recorded before status changes

## Steps

- [x] Run startup checks and preserve the existing staged/untracked worktree.
- [x] Read harness context and existing CAN gateway plan.
- [x] Inspect current EVSN CAN DDS frame contracts.
- [x] Inspect local `vcan-drivers` README, helper header, SIL test, and CAN
      utilities.
- [x] Consult QNX online documentation for raw CAN `devctl()` behavior.
- [x] Create this HAL execution plan and durable work item traceability.
- [x] Implement `evsn-runtime/can_hal` portable frame, validation, result, and
      diagnostics types.
- [x] Implement the fake backend and tests before any QNX backend.
- [x] Add CMake backend selection with one selected backend linked privately.
- [x] Implement the QNX `devctl()` backend using official QNX headers only.
- [ ] Add optional `dev-can-linux` SIL extension support only if approved.
- [x] Add CMake placeholders and clear configure failures for `tda4-fdcan`
      until vendor headers and libraries are available.
- [ ] Update `VSN-0017` CAN gateway plan to consume `evsn_can_hal`.
- [x] Add QNX/SIL smoke evidence using `dev-can-linux -L 1`, `cansend`, and
      `candump` before claiming vcan runtime behavior.
- [x] Add a QNX-only `qnx_can_hal_vcan_smoke` executable linked against
      `evsn_can_hal` to exercise `CanChannel` open/write/read/close on
      `dev-can-linux -L` endpoints.
- [ ] Add target evidence for TDA4 FD-CAN before enabling that backend in a
      production deployment.

## Verification

- [x] Command: `./init.sh check`
      Result: passed on 2026-06-22; validated 37 work items and links in 130
      Markdown files.
- [x] Command: `git diff --check`
      Result: passed on 2026-06-22.
- [x] Command: focused CTest for `can_hal` validation and fake backend tests
      Result: `./scripts/linux-test` passed all 7 CTest tests on 2026-06-22,
      including `can_hal_validation_test`, `can_hal_fake_backend_test`, and
      `can_hal_public_boundary_test`.
- [x] Command: Linux configure/build with `EVSN_CAN_BACKEND=fake`
      Result: `./scripts/linux-test` configured with the default fake backend,
      built `evsn_can_hal`, and passed all 7 CTest tests on 2026-06-22.
- [x] Command: QNX configure/build with `EVSN_CAN_BACKEND=qnx-devctl`
      Result: `./scripts/qnx-build --target evsn_can_hal` configured QNX
      AArch64 with default `qnx-devctl`, found `<sys/can_dcmd.h>`, built
      `libevsn_can_hal.a`, refreshed `out/qnx/compile_commands.json`, and
      verified the installed QNX executable identity on 2026-06-22.
- [x] Command: QNX/SIL smoke with `dev-can-linux -L 1`, `cansend`, and
      `candump`
      Result: passed on 2026-06-22 against
      `/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`;
      target identity `QNX qnx-a64 8.0.0 2025/07/30-19:17:34EDT
      QEMU_virt aarch64le`; `dev-can-linux -L 1 -u id=0,rx=1,tx=1
      -b id=0,freq=1000k` created `/dev/can0/rx0` and `/dev/can0/tx0`;
      `candump -u0,rx0` observed standard frame
      `[SFF] 124 [8] 124#1122334455667788` and extended frame
      `[EFF] 1234 [4] 1234#AABBCCDD`; smoke script exited 0 and cleaned up
      the driver/dump processes.
- [x] Command: `./scripts/qnx-build --target qnx_can_hal_vcan_smoke`
      Result: passed on 2026-06-24; built and installed
      `out/qnx/bin/qnx_can_hal_vcan_smoke` as a QNX AArch64 executable linked
      against `evsn_can_hal` with backend `qnx-devctl`.
- [x] Command: QNX/SIL HAL-linked smoke with `dev-can-linux -L 1` and
      `qnx_can_hal_vcan_smoke /dev/can0/rx0 /dev/can0/tx0`
      Result: passed on 2026-06-24 against
      `/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`;
      target identity `QNX qnx-a64 8.0.0 2025/07/30-19:17:34EDT
      QEMU_virt aarch64le`; HAL reported backend `qnx-devctl`, wrote and read
      standard frame `0x124` payload `1122334455667788` and extended frame
      `0x1234` payload `AABBCCDD`, printed diagnostics
      `opens=1 closes=1 rx=2 tx=2`, and exited 0.
- [ ] Command: TDA4 target smoke with approved FD-CAN headers/libraries and
      hardware loopback or bench CAN analyzer
      Result: blocked until target artifacts and bench contract exist.

## Acceptance Criteria

- `can_gateway` public code can build without including QNX, `dev-can-linux`,
  or TDA4 CAN headers.
- Exactly one CAN backend is selected by CMake for each build.
- Linux tests use the fake backend and require no QNX SDK or vendor CAN SDK.
- QNX vcan/SIL builds can select the QNX raw `devctl()` backend against
  `dev-can-linux` endpoints.
- TDA4 backend configuration fails clearly until vendor headers and libraries
  are supplied.
- HAL frame validation covers classic and FD lengths, DLC, standard and
  extended IDs, remote frames, unsupported FD flags, and backend capability
  mismatches.
- No dynamic allocation is required in steady-state `read()` or `write()`.
- Every fallible backend operation returns an explicit `CanStatus` plus native
  error context where available.
- Verification evidence distinguishes fake unit tests, QNX vcan/SIL tests, and
  physical TDA4 target tests.

## Risks And Blockers

- TDA4 FD-CAN headers, libraries, API contract, license terms, and exact target
  architecture are not available in this checkout.
- QNX/SIL runtime smoke with `dev-can-linux -L`, `cansend`, and `candump`
  covers vcan loopback only; it does not prove physical CAN-FD timing or
  target-controller behavior.
- Official QNX SDP 8.0 raw CAN docs show an 8-byte `struct can_msg` payload.
  Do not infer portable FD-CAN support from that API alone.
- `dev-can-linux` local vcan can prove SIL behavior, not physical CAN-FD timing
  or target-controller behavior.
- `EVSNFDCanFrame` currently lacks extended-ID, remote-frame, DLC, and FD flag
  fields. The HAL should preserve these internally, but the DDS gateway must
  reject or explicitly constrain mappings until the DDS contract is resolved.

## Evidence

- Created on 2026-06-20 from repository harness inspection, existing CAN
  gateway plan `VSN-0017`, local `vcan-drivers` inspection, and QNX SDP 8.0
  online documentation.
- Work item: `VSN-0019` in `.agents/feature_list.json`.
- Related gateway plan: `.agents/exec_plans/evsn-can-gateway.md`.
- Implemented on 2026-06-22 with subagent researcher
  `019eeda3-d4c2-7e82-b550-4e0ba3927ce4` and reviewer
  `019eedab-5439-7122-b73a-eca8466293be` evidence.
- Durable logs:
  `.agents/logs/2026-06-20-evsn-can-abstraction-layer-plan.md`
  `.agents/logs/2026-06-22-evsn-can-abstraction-layer-implementation.md`,
  `.agents/logs/2026-06-22-evsn-can-hal-qnx-sil-smoke.md`, and
  `.agents/logs/2026-06-24-evsn-can-hal-qnx-linked-smoke.md`.

## Handoff

The portable HAL, fake backend, CMake backend selector, configure-failing
optional backends, and QNX raw `devctl()` backend now build and pass local
verification. QNX/SIL vcan runtime smoke is captured both with
`dev-can-linux` utilities and with the HAL-linked `qnx_can_hal_vcan_smoke`
executable. Keep `VSN-0019` `in_progress` until TDA4 FD-CAN vendor artifacts
and bench evidence exist. Do not touch DDS gateway integration until the
`VSN-0017` RTI MCP blocker has been resolved for DDS QoS and lifecycle
decisions.
