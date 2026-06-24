# Progress

## 2026-06-24 - evsn-j784s4-mcan-driver-plan

Status: planned and blocked before implementation

Added `VSN-0038` and
`.agents/exec_plans/evsn-j784s4-mcan-libcan-driver.md` for direct QNX CAN
ownership on the Jacinto 7 J784S4 EVM.

Planning decisions:

- Treat the work as a new QNX `libcan` resource-manager driver,
  `devcan-mcan-j784s4`, exposing `/dev/can*` endpoints.
- Do not compile Linux SocketCAN `m_can` as the QNX driver.
- Use QNX CAN DDK/libcan for the driver structure.
- Use TI RTOS/PDK/CSL MCAN for hardware/register access after provenance,
  license, and import approval.
- Use TI Linux `m_can` and J784S4 device-tree content only as behavior and
  resource references unless legal approves otherwise.
- Keep RTI Connext bridge work out of scope until `VSN-0017` RTI guidance
  clears.

Durable log:

- `.agents/logs/2026-06-24-evsn-j784s4-mcan-driver-plan.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `./init.sh check` passed; harness validated 38 work items and links in 135
  Markdown files.
- `git diff --check` passed.

Remaining blockers:

- QNX CAN package, TI SDK sources, J784S4 target access, privilege/resource
  mapping, and PCAN-Router bench evidence are not present in this checkout.

## 2026-06-24 - evsn-can-hal-qnx-linked-smoke

Status: HAL-linked QNX/SIL vcan smoke captured

Added `qnx_can_hal_vcan_smoke`, a QNX-only executable linked against
`evsn_can_hal`.

Implementation:

- Added `evsn-runtime/can_hal/test/qnx_can_hal_vcan_smoke.cpp`.
- Added a QNX-only CMake target and install rule in
  `evsn-runtime/can_hal/CMakeLists.txt`.
- The executable opens RX/TX endpoints through `CanChannel`, drains stale RX,
  writes a standard classic frame and an extended classic frame, waits for the
  same frames through `CanChannel::read()`, and prints HAL diagnostics.

Verification:

- `./scripts/qnx-build --target qnx_can_hal_vcan_smoke` passed and installed
  `out/qnx/bin/qnx_can_hal_vcan_smoke`.
- Artifact identity: QNX AArch64 PIE executable, BuildID
  `4aef54f73627e6850c479c3a0b1e8974`.
- QNX/SIL target run passed on
  `/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`.
- Target identity: `QNX qnx-a64 8.0.0 2025/07/30-19:17:34EDT QEMU_virt
  aarch64le`.
- `dev-can-linux -L 1 -u id=0,rx=1,tx=1 -b id=0,freq=1000k` created
  `/dev/can0/rx0` and `/dev/can0/tx0`.
- `qnx_can_hal_vcan_smoke /dev/can0/rx0 /dev/can0/tx0` reported backend
  `qnx-devctl`, read back `0x124` payload `1122334455667788` and extended
  `0x1234` payload `AABBCCDD`, printed `opens=1 closes=1 rx=2 tx=2`, and
  exited 0.
- `bash evsn-runtime/can_hal/test/check_public_boundary.sh evsn-runtime/can_hal`
  passed.
- `git diff --check` passed.
- `./scripts/linux-test` passed all 7 CTest tests.

Durable log:

- `.agents/logs/2026-06-24-evsn-can-hal-qnx-linked-smoke.md`

Remaining gap:

- This is still vcan/SIL evidence. `VSN-0019` remains `in_progress` until
  TDA4 FD-CAN vendor artifacts and bench or hardware evidence exist.

## 2026-06-22 - evsn-can-hal-qnx-sil-smoke

Status: QNX/SIL vcan runtime smoke captured

Ran the VSN-0019 target-tester pass against
`/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`.

Evidence:

- Target identity: `QNX qnx-a64 8.0.0 2025/07/30-19:17:34EDT QEMU_virt
  aarch64le`.
- Built `vcan-drivers` `dev-can-linux`, `cansend`, and `candump` as QNX
  AArch64 executables in the existing `dev-env-builder` container.
- Deployed the tools to `/tmp/evsn-vcan-smoke` over SSH on localhost port 2222.
- Ran `dev-can-linux -L 1 -u id=0,rx=1,tx=1 -b id=0,freq=1000k`.
- Verified `/dev/can0/rx0` and `/dev/can0/tx0` appeared.
- Ran `candump -u0,rx0` while `cansend -u0,tx0` transmitted standard frame
  `0x124` payload `1122334455667788` and extended frame `0x1234` payload
  `AABBCCDD`.
- `candump` observed `[SFF] 124 [8] 124#1122334455667788` and
  `[EFF] 1234 [4] 1234#AABBCCDD`.
- The smoke script exited 0 and cleaned up the driver and dump processes.

Durable log:

- `.agents/logs/2026-06-22-evsn-can-hal-qnx-sil-smoke.md`

Remaining gap:

- `VSN-0019` stays `in_progress` until TDA4 FD-CAN vendor artifacts, API
  contract, target architecture, and bench or hardware evidence exist.

## 2026-06-22 - evsn-can-abstraction-layer-implementation

Status: core HAL implemented; TDA4 evidence still pending

Implemented `VSN-0019` core CAN abstraction layer under
`evsn-runtime/can_hal/`.

Implementation decisions:

- Added a DDS-free portable CAN HAL API with fixed-size frame/config storage,
  explicit `CanStatus`/native-error results, capabilities, diagnostics, and
  non-throwing channel operations.
- Added centralized validation for classic/FD payload lengths, DLC conversion,
  standard and extended IDs, remote-frame constraints, FD flags, and backend
  capability mismatches.
- Added a deterministic fake backend for Linux tests with fixed-capacity
  injected RX frames, recorded TX frames, and forced status paths.
- Added `EVSN_CAN_BACKEND` CMake selection, defaulting to `fake` for Linux and
  `qnx-devctl` for QNX; `qnx-dev-can-linux` and `tda4-fdcan` fail clearly until
  approval/vendor evidence exists.
- Added a QNX raw `devctl()` backend using official `<sys/can_dcmd.h>` only,
  mapping RX `EAGAIN` to `no_frame` and TX `EAGAIN` to `tx_full`.
- Kept `dev-can-linux/commands.h` out of EVSN HAL public headers and source.
- Did not expose QNX raw timestamps as monotonic HAL timestamps because the
  QNX raw `can_msg` timestamp unit/clock is not yet accepted as a monotonic-ns
  contract.

Subagents:

- Researcher `019eeda3-d4c2-7e82-b550-4e0ba3927ce4` confirmed QNX evidence,
  GPL/header-boundary risk, SIL-only vcan scope, backend-default risk, and
  remaining pass blockers.
- Reviewer `019eedab-5439-7122-b73a-eca8466293be` found FD-flag and timestamp
  issues; both were fixed before final verification.

Durable log:

- `.agents/logs/2026-06-22-evsn-can-abstraction-layer-implementation.md`

Verification:

- `./scripts/linux-test` passed all 7 CTest tests, including
  `can_hal_validation_test`, `can_hal_fake_backend_test`, and
  `can_hal_public_boundary_test`.
- `./scripts/qnx-build --target evsn_can_hal` passed with default
  `EVSN_CAN_BACKEND=qnx-devctl`, found `<sys/can_dcmd.h>`, built
  `libevsn_can_hal.a`, refreshed `out/qnx/compile_commands.json`, and verified
  QNX artifact identity through the existing wrapper.
- `bash evsn-runtime/can_hal/test/check_public_boundary.sh evsn-runtime/can_hal`
  passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 37 work items and links in 130 Markdown
  files.

Remaining gaps:

- QNX/SIL vcan runtime smoke has been captured separately; it does not prove
  physical CAN-FD timing or target-controller behavior.
- TDA4 FD-CAN remains blocked on vendor headers, libraries, ABI, license,
  target architecture, and bench evidence.

## 2026-06-22 - evsn-core-equivalence-recheck

Status: planning-equivalent for core functionality

Re-ran the core functionality equivalence check after shelving the driver
display to `VSN-0037`.

Planning decisions:

- Read-only reviewer subagent `019eed8a-95e4-7243-9e47-afd6d9313bc6`
  returned `core-planning-equivalent`.
- Core EVSN plans cover the active EMBD-v2-HighLevel network/control surfaces
  at planning level, minus explicitly retired systems and the shelved display.
- The old USB/serial display output path and `centre_display` remain out of the
  MVP; future HDMI Qt/QML display work is last-priority `VSN-0037`.
- Corrected stale BMS summary wording to keep the Orion 2 BMS range aligned at
  `0x620..0x629`.
- Hardened the cloud bridge and regression plans so old cloud-originated motor
  reset, motor power, motor limit, torque/current/velocity, drive-enable, and
  final CAN0 command paths are rejected unless a future approved
  command-authority plan exists.

Residual risks:

- This is planning equivalence only, not implemented, HIL, or vehicle-verified
  equivalence.
- RTI MCP guidance, QNX target/timing evidence, package implementation, and
  `VSN-0033` regression execution remain required before claiming runtime
  equivalence.

Durable log:

- `.agents/logs/2026-06-22-evsn-core-equivalence-recheck.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- Stale wording/regression scan passed with no remaining matches.
- `git diff --check` passed.
- `./init.sh check` passed, validating 37 work items and links in 129 Markdown
  files.

## 2026-06-22 - evsn-driver-display-qt-frontend

Status: shelved before implementation

Updated display planning after the user clarified the driver display should be
done last and should move from the old USB/serial display approach to HDMI plus
a Qt frontend.

Planning decisions:

- Added `VSN-0037` and
  `.agents/exec_plans/evsn-driver-display-qt-frontend.md`.
- The old USB/serial `display_driver` output path and `centre_display` remain
  absent from the MVP runtime.
- Driver display is not permanently retired; it is deferred until after the
  network/control MVP and regression foundations are complete.
- The future display target is a QNX HDMI Qt Quick/QML frontend consuming
  copied display snapshots only.
- The display process must have no control authority, no final CAN0 motor
  command path, no real-time-loop participation, and no back-pressure into
  control, CAN0, or telemetry ingestion paths.
- `VSN-0026` remains lighting-only.

Durable log:

- `.agents/logs/2026-06-22-evsn-driver-display-qt-frontend.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 37 work items and links in 128 Markdown
  files.

## 2026-06-22 - evsn-hardware-equivalence-matrix

Status: planned before implementation

Updated the EVSN network migration plans to treat unchanged hardware as a
first-class compatibility contract copied from EMBD-v2-HighLevel launch and
configuration sources.

Planning decisions:

- Added a hardware-equivalence matrix to
  `.agents/exec_plans/evsn-network-port-roadmap.md` covering bus, frame ID,
  direction, old ROS2 topic names, new QNX process owner, candidate DDS topic,
  and regression test ID.
- Kept CAN0 as the drive-control bus owned by `evsn_can0_drive_bus_rt`; final
  motor commands stay off DDS and out of the generic CAN gateway.
- Required CAN0 receive fanout to local subscribers so PedalBox, BMS, motor
  telemetry, brake pressure, solar current, GNSS, and optional tyre-pressure
  decoders can preserve old launch/config behavior without each owning CAN0.
- Updated per-area plans so CAN1 lighting/MPPT/irradiance, CAN2 proprietary
  Audi headlights, CAN3 steerwheel, CAN0 GPS, CANopen PedalBox, controllers,
  IDL, CAN HAL, and regression plans all reference the matrix.
- Added a steering ECU/EPAS retired row because EMBD-v2-HighLevel `can_broker`
  includes steering frames but the user confirmed the steering actuator motor
  has been removed. Those frames are no longer an MVP equivalence blocker, and
  regression must prove there is no active route or command path.
- Deferred driver display out of MVP: the old USB/serial `display_driver`
  output path remains absent, future HDMI Qt display belongs to `VSN-0037`, and
  VSN-0026 now covers active lighting drivers only.
- Reviewer subagent `019eed7c-4c11-7592-8cd7-41ed5a9ce327` returned
  `planning-equivalent`: no active non-retired EMBD-v2-HighLevel network
  surface was missing from the EVSN plans.
- Added a launch-file traceability checklist mapping each `system_launch.xml`
  surface to an EVSN owner or explicit retirement/optional decision.

Durable log:

- `.agents/logs/2026-06-22-evsn-hardware-equivalence-matrix.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 36 work items and links in 126 Markdown
  files.
- Re-ran the same checks after steering ECU retirement, display deferral,
  launch-traceability, and subagent-review updates; all passed.

## 2026-06-21 - can0-drive-control-bus-correction

Status: planned before implementation

Corrected the CAN0 ownership model after reading the EMBD-v2-HighLevel
physical architecture diagram. This was refined on 2026-06-22 against
EMBD-v2-HighLevel launch/config sources and the hardware-equivalence matrix.

Planning decisions:

- CAN0 is now documented as the drive-control bus, not a motor-only bus.
- `evsn_motor_control_rt` computes final motor commands.
- A separate real-time `evsn_can0_drive_bus_rt` process owns CAN0 HAL writes.
- Final motor command frames are handed from motor control to the CAN0 bus
  owner through bounded QNX-local IPC, not DDS and not the generic gateway.
- PedalBox remains on CAN0 but is receive-first. PedalBox TX is disabled unless
  the old CANopen config, EDS/DCF, device documentation, or bench evidence
  proves NMT/SDO/SYNC or another management write is required.

Durable log:

- `.agents/logs/2026-06-21-can0-drive-control-bus-correction.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- Stale CAN0 motor-only wording search passed with no remaining matches in
  active plans, work items, progress, or logs.
- `./init.sh check` passed, validating 36 work items and links in 125 Markdown
  files.

## 2026-06-21 - evsn-canopen-pedalbox-option2

Status: planned before implementation

Updated `VSN-0035` to select implementation option 2 for the CANopen PedalBox
CAN0 conflict.

Decision:

- Keep PedalBox on CAN0.
- A single approved CAN0 owner/supervisor owns all CAN0 transmission.
- Motor command frames have absolute priority.
- The PedalBox manager must not directly open or write CAN0 TX.
- PedalBox CANopen management traffic becomes bounded, whitelisted,
  rate-limited requests to the CAN0 owner/supervisor.

Durable log:

- `.agents/logs/2026-06-21-evsn-canopen-pedalbox-option2.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 36 work items and links in 124 Markdown
  files.

## 2026-06-20 - evsn-gps-can-driver-port

Status: planned before implementation

Created `VSN-0036` and `.agents/exec_plans/evsn-gps-can-driver-port.md` for
porting the legacy GPS driver out of ROS2 `can_broker` into the EVSN CAN
gateway format.

Planning decisions:

- Keep the generic CAN broker/gateway raw-frame only.
- Port legacy GPS payload decode into `evsn_gnss_can_driver`, a CAN-ID-owning
  package that consumes raw frames for standard IDs `0x700..0x704`.
- Replace legacy scalar ROS2 topics such as `/gps/lat`, `/gps/long`, and
  `/car/speed/gps` with a typed GNSS/navigation state under `VSN-0021`.
- Block final DDS type/topic/QoS implementation until RTI Connext MCP guidance
  is recorded.

Durable log:

- `.agents/logs/2026-06-20-evsn-gps-can-driver-port.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 36 work items and links in 123 Markdown
  files.

## 2026-06-20 - evsn-canopen-pedalbox-qnx-port

Status: planned before implementation

Created `VSN-0035` and
`.agents/exec_plans/evsn-canopen-pedalbox-qnx-port.md` for a QNX-native
CANopen PedalBox manager.

Planning decisions:

- Split PedalBox CANopen behavior out of the generic `VSN-0025` input-driver
  plan because NMT, heartbeat, TPDO mapping, stack selection, and bus ownership
  require separate acceptance criteria.
- Planned the package to use `evsn_can_hal` from `VSN-0019`, keeping QNX CAN
  APIs and third-party stack headers out of public EVSN interfaces.
- Recommended CANopenNode as the first-pass open-source stack candidate, with
  Lely CANopen as the fallback after QNX build, allocation, and dependency
  evaluation.
- Recorded the initial CAN0 blocker. This was superseded on 2026-06-21 after
  reading the EMBD-v2-HighLevel physical architecture diagram: CAN0 is the
  shared drive-control bus, and `evsn_can0_drive_bus_rt` is the planned
  separate CAN0 writer process.

Durable log:

- `.agents/logs/2026-06-20-evsn-canopen-pedalbox-qnx-port.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 35 work items and links in 121 Markdown
  files.

## 2026-06-20 - evsn-network-regression-tests

Status: planned before implementation

Added a separate strict regression and integration test-suite plan for the
network migration:

- Created or strengthened
  `.agents/exec_plans/evsn-network-regression-test-suite.md` as `VSN-0033`.
- Added `Strict Verification Gate` sections to the network implementation
  plans so smoke/unit tests alone are not enough to close a package.
- Required package evidence to link to `VSN-0033`, including deterministic
  smoke, pure unit, middleware/adapter integration, negative regression,
  platform/configuration, logs/traces, exit status, residual risks, and known
  gaps.
- Expanded `VSN-0033` dependencies to cover the CAN HAL plus network port work
  from `VSN-0021` through `VSN-0032`.

Durable log:

- `.agents/logs/2026-06-20-evsn-network-regression-tests.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `./init.sh check` passed, validating 34 work items and links in 119 Markdown
  files.
- `git diff --check` passed.

## 2026-06-20 - rtiperftest-regression-harness-plan

Status: planned before implementation

Created `VSN-0034` and `.agents/exec_plans/rtiperftest-regression-harness.md`
for introducing RTI's public `rticommunity/rtiperftest` repository as an
optional performance-characterization harness that can run before, after, or
around the EVSN regression suite.

Planning evidence:

- RTI MCP guidance recorded for isolated DDS domains, explicit transport/NIC
  selection, benchmark-only QoS/configuration, subscriber-before-publisher
  startup, and treating latency/throughput as performance evidence rather than
  functional correctness.
- Official RTI Perftest GitHub/docs/release pages consulted.
- Current QNX Product Documentation and QNX SDP 8.0 docs consulted for optional
  QNX target build/run context.

Traceability:

- Added the pre-existing `VSN-0033` network regression-suite plan to
  `.agents/feature_list.json`.
- Durable log:
  `.agents/logs/2026-06-20-rtiperftest-regression-harness.md`.

Verification:

- `jq empty .agents/feature_list.json` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 34 work items and links in 119 Markdown
  files.

## 2026-06-20 - evsn-network-port-plans

Status: planned before implementation

Created execution plans for porting active EMBD-v2-HighLevel network, driver,
controller, telemetry, CAN, motor, and IDL functionality into the EVSN QNX/RTI
runtime:

- Roadmap and ignore-list inventory.
- ROS2 message to EVSN IDL/type contract plan.
- CAN broker/gateway adaptation plan for the current EVSN raw CAN design.
- QNX motor-control critical-path plan that owns CAN0 directly and excludes DDS
  and the generic CAN gateway from final motor commands.
- Driver/controller plans for BMS, pedal, steerwheel, display/headlight/body
  lighting, sensors/MPPTs, drive, cruise, and non-critical body control.
- Generic energy telemetry plan using ITTIA DB instead of InfluxDB.
- Generic cloud telemetry bridge plan with branded names removed and
  proprietary Audi headlight material excluded from public cloud artifacts.

RTI MCP status:

- Tool discovery exposed no RTI Connext MCP in this session, so every DDS/IDL
  implementation plan records RTI MCP guidance as a blocker before final DDS
  type, topic, QoS, generated-code, WaitSet/listener, or participant work.

QNX online documentation consulted:

- QNX Product Documentation index.
- QNX SDP 8.0 documentation.
- Scheduling, `pthread_setschedparam()`, `clock_nanosleep()`, `ThreadCtl()`,
  processor affinity/runmask, and `tracelogger` pages.

ITTIA references consulted:

- ITTIA QNX platform page.
- ITTIA DB product page.

Durable log:

- `.agents/logs/2026-06-20-evsn-network-port-plans.md`

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `./init.sh check` passed, validating 32 work items and links in 115 Markdown
  files.
- `git diff --check` passed.

## 2026-06-20 - evsn-node-qnx-qemu-runtime-evidence

Status: complete

Completed the remaining `VSN-0015` QNX runtime evidence using the supplied QEMU
target at `/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`:

- Added QNX-only `qnx_evsn_node_ham_smoke`, linked through `evsn_node` and
  installed by the repository QNX CMake path.
- Built it with `./scripts/evsn build --target qnx_evsn_node_ham_smoke`.
- Booted QNX 8.0.0 aarch64le in QEMU, staged HAM from the QNX SDP target image,
  and ran clean, missed-heartbeat stall, and death/restart smoke modes.
- Copied marker, timing, and QNX `tracelogger` evidence to
  `.agents/evidence/VSN-0015-qnx-ham/`.
- Marked `VSN-0015` passing.

Verification:

- Clean mode produced attach/first-tick/alive/detach markers, 25 timer ticks,
  and a 41,075-byte QNX trace capture.
- Stall mode produced the HAM high-action marker with max interval
  `3599988 us`.
- Death mode exited initial process with status `137`; HAM restarted the
  repository-linked executable and the restarted process detached cleanly.

## 2026-06-20 - rti-qnx-guidance-guard

Status: complete

Strengthened the harness external-guidance guard:

- RTI-Connext based node and DDS contract work must run the RTI Connext MCP and
  record the evidence before implementation.
- QNX-facing implementation or review must consult current online QNX
  documentation and stop if those docs cannot be reached.
- `./init.sh check` now verifies the stronger guard text remains present.

Verification:

- `./init.sh check` passed, including the RTI/QNX guard checks.

## 2026-06-20 - evsn-node-review-rewrite-implementation

Status: implemented locally; blocked on target runtime evidence

Implemented the evidence-backed EVSNNode runtime rewrite from
`.agents/exec_plans/evsn-node-review-rewrite.md`:

- RTI Connext MCP guidance was recorded for WaitSet ownership, loaned samples,
  typed vs Any endpoints, parameter request/reply, QoS, `ReplierParams`, and
  HAM restart rediscovery.
- Current QNX SDP 8.0 online documentation was recorded for HAM, timing,
  scheduling, signal waiting, and trace instrumentation.
- `EVSNNode` now uses typed subscription records that own DataReaders and
  StatusConditions, lexical loan handling, bounded topic validation, explicit
  timer catch-up, bounded WaitSet waits, and heartbeat-after-progress behavior.
- Runtime-owned parameter DDS entities now use explicit QoS/resource limits,
  and parameter request/reply construction uses `rti::request::ReplierParams`.
- QNX HAM environment parsing and `/proc/self/exefile` handling were tightened.
- The node supervision test now runs through the shared-memory DDS wrapper and
  covers invalid construction-time API inputs plus WaitSet subscriber dispatch.

Verification:

- `./scripts/test-no-evsnnode-mqtt` passed.
- `./scripts/linux-test --clean` passed a clean container build and all 4 CTest
  tests.
- `VERIFY_QNX=1 ./init.sh verify` passed harness checks, Linux build/tests, and
  QNX cross-build for `armv8QNX8.0qcc_cxx12.2.0`, including
  `qnx_ham_node_supervisor.cpp` and `libevsn_node.a`.

Remaining blocker:

- No approved QNX target or QEMU runtime contract is configured for HAM attach,
  heartbeat, missed-heartbeat handling, restart, clean detach, and timing/trace
  evidence.

## 2026-06-20 - evsn-can-abstraction-layer-plan

Status: planned before implementation

Created `.agents/exec_plans/evsn-can-abstraction-layer.md` and work item
`VSN-0019` for a target-selectable CAN HAL:

- Public gateway code links `evsn_can_hal` and does not include QNX,
  `dev-can-linux`, or future TDA4 CAN headers directly.
- Planned backends are `fake`, `qnx-devctl`, `qnx-dev-can-linux`, and
  `tda4-fdcan`, selected by `EVSN_CAN_BACKEND`.
- Linux tests use the fake backend.
- QNX vcan/SIL tests use raw `devctl()` against `dev-can-linux -L` endpoints.
- TDA4 FD-CAN remains a configure-failing placeholder until vendor headers,
  libraries, API contract, license terms, architecture, and bench evidence are
  available.
- `VSN-0017` now depends on `VSN-0019` and should consume the HAL before direct
  gateway hardware integration.

QNX online documentation consulted:

- QNX Product Documentation index.
- QNX SDP 8.0 documentation.
- CAN raw-frame `devctl()` command pages.
- C library `devctl()` reference.

Blockers:

- TDA4 FD-CAN headers/libraries/API contract are unavailable.
- `docker/.env` does not define `DEV_ENV_BUILDER_IMAGE`, so live QNX build
  verification cannot run.

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `./init.sh check` passed, validating 19 work items and links in 100 Markdown
  files.
- `git diff --check` passed.

Durable log:

- `.agents/logs/2026-06-20-evsn-can-abstraction-layer-plan.md`

## 2026-06-20 - vcan-intellisense

Status: complete

Added the `vcan-drivers/dev-can-linux` include root to the host-normalized
IntelliSense compile-database setup so QNX CAN gateway work can include
`commands.h` from the checked-out `vcan-drivers` tree.

Updated both compile-command rewriters:

- `tools/evsn_cli/compile_commands.py`
- `scripts/rewrite-compile-commands.mjs`

Verification:

- `python3 -m py_compile tools/evsn_cli/*.py` passed.
- `node --check scripts/rewrite-compile-commands.mjs` passed.
- `python3 -m unittest discover -s tests/evsn_cli` passed with 14 tests.
- `./scripts/evsn compile-commands --dry-run` passed.
- `./init.sh check` passed, validating 18 work items and links in 97 Markdown
  files.
- `git diff --check` passed.

## 2026-06-20 - vsn-work-item-ids

Status: complete

Migrated the durable work-item namespace from the inherited project format to
S7-EVSN `VSN-XXXX` IDs:

- Existing work item IDs now use `VSN-0001` through `VSN-0018`.
- Dependency references and current handoff references were updated to the same
  format.
- `AGENTS.md`, the execution-plan template, subagent prompt templates, schema,
  and feature-list validator now require uppercase `VSN-XXXX` IDs.

Verification:

- `! rg -n '[m]jolnir-[[:digit:]#x]{3}' AGENTS.md .agents
  init.sh README.md scripts tests tools cmake CMakeLists.txt` passed with no
  matches.
- `./init.sh check` passed, validating 18 work items and links in 76 Markdown
  files.
- `git diff --check` passed.

## 2026-06-20 - evsn-node-rewrite-blocker-check

Status: blocked before implementation

Rechecked whether the EVSNNode rewrite can begin after Docker `.env` setup:

- RTI Connext MCP remains unavailable. `tool_search` exposed no RTI Connext MCP,
  and the plugin install candidate list did not include an RTI Connext
  MCP/plugin.
- The `rti-connext-dds` skill and active execution plan still require RTI MCP
  guidance before DDS/node design decisions or source implementation.
- `docker/.env` exists, but `./scripts/evsn doctor` reports
  `DEV_ENV_BUILDER_IMAGE` is missing.
- The current `.env` variable key is `QNX_BUILD_IMAGE`; the repository compose
  and EVSN CLI contract expects `DEV_ENV_BUILDER_IMAGE`.
- `./scripts/evsn build --target evsn_node` still fails at preflight until
  `DEV_ENV_BUILDER_IMAGE` is defined.

No runtime source files were edited.

## 2026-06-20 - evsn-qnx-build-cli-originality-audit

Status: complete locally; live QNX verification still pending

Checked the EVSN QNX build CLI against the S130 build tools:

- Exact-line audit compared 9 EVSN build-tool files against 7 S130 tool files.
- Shared exact nonblank stripped lines: 84.
- Substantive copied-line candidates after filtering boilerplate/generic CLI
  lines: 0.
- Longest contiguous shared block: 2 generic lines.
- No-context explorer subagent `019ee122-cea6-7091-9d1a-57c291620580`
  reviewed S130 CLI A versus EVSN CLI B and judged EVSN stronger for QNX build
  orchestration.
- Reviewer-flagged RTI architecture filter `QNX|QOS` was corrected to the
  case-insensitive `qnx|nto` filter and covered by a unit test.

Verification:

- `python3 -m py_compile tools/evsn_cli/*.py` passed.
- `python3 -m unittest discover -s tests/evsn_cli` passed with 14 tests.
- `bash -n init.sh scripts/*` passed.
- `./scripts/evsn build --dry-run --target evsn_node` passed and rendered
  `grep -Ei "qnx|nto"` without `QOS`.
- `./init.sh check` passed, validating 17 work items and links in 75 Markdown
  files.
- `git diff --check` passed.

## 2026-06-20 - evsn-can-gateway-plan

Status: blocked before implementation

Created `.agents/exec_plans/evsn-can-gateway.md` for an EVSN CAN gateway:

- One gateway process instance owns one configured CAN bus.
- RX uses a configured QNX CAN endpoint and publishes raw frames to
  `/can<bus>/from_can_bus` by default.
- TX subscribes to `/can<bus>/to_can_bus` by default and writes accepted frames
  to a configured QNX CAN endpoint.
- The default topic names copy EMBD-v2-HighLevel's socketcan bridge convention
  for non-CAN0 buses, while CAN0 is superseded by the dedicated
  `evsn_can0_drive_bus_rt` owner and exposes only approved local TX requests
  plus read-side fanout/mirror paths.
- The gateway keeps the raw CAN boundary; CAN-ID-owning nodes still perform DBC
  pack/unpack locally.
- CAN 2.0 topics use `EVSNCan20Frame`; FD-capable topics use `EVSNFDCanFrame`
  only where the current metadata limits are acceptable or explicitly resolved.
- DDS callbacks enqueue to a bounded TX queue and never perform hardware I/O.
- QNX raw-frame handling uses nonblocking receive and checked `devctl()`
  results.

Evidence sources:

- Local Codex chat history for the prior raw CAN gateway discussion.
- Local SR-Mjolnir CAN gateway, CAN 2.0, IoT bridge, topic registry, and motor
  driver context.
- Local EMBD-v2-HighLevel `ros2_socketcan` and `can_broker` topic naming.
- Current EVSN runtime and DDS IDL files.
- Local `dev-can-linux` helper API, README, and raw-frame work-item evidence.
- QNX Product Documentation and QNX SDP 8.0 online docs.
- RTI Connext MCP topic-name guidance, which reported no DDS-specific blocker
  to the bus-first rename and no required DDS partitions.

Current blocker:

- The full RTI Connext MCP QoS/lifecycle guidance query timed out after
  approximately 120 seconds. Source implementation and final DDS QoS decisions
  remain blocked until a successful RTI MCP result is recorded in the plan.

Durable log:

- `.agents/logs/2026-06-20-evsn-can-gateway-plan.md`

## 2026-06-20 - evsn-qnx-build-cli-implementation

Status: blocked for live QNX verification

Implemented the repo-local EVSN QNX CLI:

- `./scripts/evsn` is the preferred entrypoint.
- Python stdlib implementation lives under `tools/evsn_cli/`.
- Focused unit tests live under `tests/evsn_cli/`.
- Legacy `scripts/qnx-build`, `scripts/qnx-clean`, `scripts/qnx-shell`, and
  `scripts/qnx-compile-commands` delegate to the EVSN CLI.
- README documents common EVSN CLI commands.
- Work-item traceability lives in `VSN-0016`.
- Durable log:
  `.agents/logs/2026-06-20-evsn-qnx-build-cli-implementation.md`.

Verification:

- `python3 -m py_compile tools/evsn_cli/*.py` passed.
- `python3 -m unittest discover -s tests/evsn_cli` passed with 8 tests.
- `bash -n init.sh scripts/*` passed.
- `./scripts/evsn help` passed.
- `./scripts/evsn doctor --offline` passed.
- `./scripts/evsn build --dry-run --target evsn_node` passed.
- `./scripts/evsn compile-commands --dry-run` passed.
- `./scripts/evsn shell --dry-run` passed.
- `./scripts/evsn verify --dry-run --target evsn_node` passed.
- Legacy wrapper dry-runs passed.
- `./init.sh check` passed, validating 16 work items and links in 73 Markdown
  files.
- `git diff --check` passed.

Blocker:

- `docker/.env` is missing, so live QNX build and QNX verification are pending.

## 2026-06-20 - evsn-node-review-rewrite

Status: blocked before implementation

Created `.agents/exec_plans/evsn-node-review-rewrite.md` for an evidence-backed
review and rewrite of `evsn-runtime/evsn_node/`:

- Uses `qnx-cpp-rtos`, `qnx-cpp-performance`, and `rti-connext-dds`.
- Requires RTI Connext MCP guidance before DDS/node design decisions or source
  implementation.
- Requires current QNX online documentation evidence before QNX-facing runtime
  review or implementation.
- Defines review checkpoints for DDS contracts, timing, allocation, blocking,
  QNX HAM supervision, parameter service behavior, and test coverage.
- Defines rewrite checkpoints for initialization/runtime separation, bounded
  spin behavior, deterministic executor design, explicit QoS, adapter seams, and
  verification evidence.
- Requires the copied/ported core EVSNNode implementation to be replaced by a
  newly designed internal architecture, not a superficial rename or mechanical
  rewrite.
- Requires source-provenance/originality review evidence and justification for
  any retained public API shape, file path, generated/vendor code, or
  unavoidable RTI/QNX API usage.

Current blocker:

- RTI Connext MCP is not callable in this Codex session, and no exact install
  candidate was available.

Durable log:

- `.agents/logs/2026-06-20-evsn-node-review-rewrite-plan.md`

Verification:

- `./init.sh check` passed, validating 15 work items and links in 72 Markdown
  files.
- `git diff --check` passed.
- After the originality-boundary update, `./init.sh check` and
  `git diff --check` passed again.

## 2026-06-20 - private-license

Status: complete

Added a root proprietary license for the repository:

- `LICENSE` states that the repository is proprietary and confidential, with no
  permission granted without express prior written approval.
- `README.md` now has a license section pointing to the root license.
- Work-item traceability lives in `VSN-0014`.
- Durable log: `.agents/logs/2026-06-20-private-license.md`.

Verification:

- `./init.sh check` passed, validating 14 work items and links in 70 Markdown
  files.
- `git diff --check` passed.

## 2026-06-20 - sr-mjolnir-oai-layout-port

Status: complete

Created an OAI-style agent harness in this checkout and populated it with
SR-Mjolnir harness content:

- Root startup and working rules live in `AGENTS.md`.
- Work-item state lives in `.agents/feature_list.json`.
- Execution plans live in `.agents/exec_plans/`.
- Durable logs live in `.agents/logs/`.
- SR-Mjolnir project context lives in `.agents/agent-context.md`.
- Subagent role contracts live in `.agents/subagents/README.md`.
- SR-Mjolnir canonical skills live in `.agents/skills/`.
- Onboarding and QNX HAM smoke-test material lives in `.agents/tooling/`.

Verification:

- `./init.sh check` validates required files, JSON, SR work-item structure,
  Markdown links, shell syntax, and Git status.
- `./init.sh check` passed on 2026-06-20, validating 13 work items and links in
  56 Markdown files.

Next action:

- For the next real task, select or add a work item in `.agents/feature_list.json`
  and create an execution plan in `.agents/exec_plans/` if the work is
  feature-level, safety-relevant, cross-module, or multi-session.

## 2026-06-20 - dev-env-builder-port

Status: complete

Ported the Docker builder environment and a generic AWS CodeBuild stack:

- Generic Docker environment lives in `docker/`.
- Generic AWS CDK app lives in `Dev-Env-Builder/`.
- The AWS app provisions only the support surface required to build and publish
  the Docker environment: S3 input bucket, ECR repository, CodeBuild role,
  CloudWatch log group, and one CodeBuild project.
- CodePipeline, devboxes, Lambda APIs, CodeArtifact, GitHub OIDC roles, and
  organization-specific IAM groups were left out.

Verification:

- New Docker/AWS files contain no project-specific source names.
- `npm run build`, `npm test -- --runInBand`, and `npm run synth` passed in
  `Dev-Env-Builder/`.
- AWS MCP CloudFormation validation passed with 0 errors, 0 warnings, and
  0 info messages.

## 2026-06-20 - evsn-runtime-port

Status: complete

Copied the SR-Mjolnir runtime into `evsn-runtime/` and renamed the copied
runtime/build surface to EVSN terminology:

- `sr_node` became `evsn_node`.
- `SRNode` became `EVSNNode`.
- DDS type filenames and generated type names use `evsn_`/`EVSN` prefixes.
- Minimal build support lives in `cmake/` and `scripts/`.
- Root CMake entrypoint lives in `CMakeLists.txt`.

Verification:

- Old-name scan over `evsn-runtime`, `cmake`, `scripts`, and `CMakeLists.txt`
  returned no matches.
- `bash -n scripts/* init.sh`, `./init.sh check`,
  `./scripts/test-no-evsnnode-mqtt`, `node --check
  scripts/rewrite-compile-commands.mjs`, and `git diff --check` passed.
- Native CMake configure reached the expected `RTIConnextDDS` package lookup
  failure because host `NDDSHOME`/`RTI_HOME` are unset; full compile needs the
  Docker builder or a configured RTI host environment.

## 2026-06-20 - evsn-qnx-build-cli

Status: complete locally; live QNX verification pending

Created `.agents/exec_plans/evsn-qnx-build-cli.md` for a clean-sheet,
repo-local EVSN CLI:

- QNX-only build orchestration.
- CMake with Ninja only.
- No `.sunswift-evsn` marker or replacement auth sentinel.
- Default Docker builder backend using the current QNX build environment.
- Explicit subagent roles for implementation, QNX verification review, and
  documentation/migration review.

Planning evidence:

- Legacy S130 behavior inventory from subagent
  `019ee0fa-7e9e-7f02-8ed0-67fe15bbe0bc`.
- Current repo build integration review from subagent
  `019ee0fa-9219-7290-9639-d76f2110803e`.
- Durable log: `.agents/logs/2026-06-20-evsn-qnx-build-cli-plan.md`.

Implementation status:

- QNX online documentation evidence was added to the execution plan.
- Local CLI implementation, tests, dry-runs, wrapper delegation, and README docs
  are now in place. Live QNX build is blocked by missing `docker/.env`.
- Implementation, QNX verification, and documentation subagents were used.
- Reviewer findings were fixed before final local verification.

## 2026-06-20 - external-doc-guards

Status: complete

Restored strict external guidance guards in the harness:

- RTI Connext node and DDS contract work must consult the RTI Connext MCP and
  record the evidence before implementation.
- QNX-facing implementation or review must consult current online QNX
  documentation and record the URLs or sections used.
- `init.sh check` now validates that the root instructions, relevant skills,
  execution-plan template, and subagent guidance still carry those guard
  anchors.

Durable log:

- `.agents/logs/2026-06-20-external-doc-guards.md`
