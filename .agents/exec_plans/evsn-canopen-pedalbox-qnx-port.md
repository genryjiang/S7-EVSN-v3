# VSN-0035 - QNX CANopen PedalBox Port

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned; implementation blocked on CAN0 drive-control bus owner
contract, stack approval, and target timing evidence

## Scope

Port the legacy CANopen PedalBox bring-up into the EVSN QNX runtime as a
QNX-native PedalBox manager. This work replaces the old ROS2 `canopen_core`
device-manager launch path for the PedalBox with a small, testable CANopen
process that reads PedalBox TPDO input, tracks CANopen liveness, and produces a
typed EVSN pedal input snapshot.

The first-pass target is the legacy PedalBox node from EMBD-v2-HighLevel:

- CANopen master node ID: `1`
- PedalBox node ID: `5`
- Legacy bitrate: `500 kbit/s`
- Legacy TPDO COB-ID: `0x185`
- Legacy EDS/DCF source: `5_PedalBox.eds`
- Mapped PedalBox objects to confirm and freeze: `0x2000` accelerator,
  `0x2001` brake, and `0x2002` brake switch
- Heartbeat producer expectation: `1000 ms`

The new package should publish `EVSNPedalState` when the existing IDL is
sufficient. If extra CANopen diagnostics require a type change, that change
must move through `VSN-0021` with RTI Connext MCP guidance before implementation.

## Non-Goals

- Do not port ROS2 `canopen_core`, `ros2_canopen`, ROS2 launch files, or ROS2
  lifecycle behavior.
- Do not build a generic all-device CANopen manager unless the PedalBox cannot
  be supported safely with a bounded PedalBox-specific manager.
- Do not parse EDS/DCF files dynamically in a real-time path.
- Do not route final motor commands through DDS, the generic CAN gateway, the
  PedalBox manager, telemetry, or cloud paths.
- Do not let the PedalBox process directly write the CAN0 device. With the
  selected CAN0 model, all CAN0 transmission goes through the separate
  real-time CAN0 drive-control bus owner.
- Do not add PedalBox CANopen TX unless the EMBD-v2-HighLevel CANopen
  configuration, EDS/DCF, device documentation, or bench evidence shows it is
  required.
- Do not vendor open-source code until version pinning, license, provenance,
  build flags, and QNX cross-build evidence are recorded.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/exec_plans/evsn-can-abstraction-layer.md`
- `.agents/exec_plans/evsn-input-drivers-port.md`
- `.agents/exec_plans/evsn-motor-control-critical-path.md`
- `.agents/exec_plans/evsn-network-regression-test-suite.md`
- `evsn-runtime/dds_types/custom_types/evsn_pedal/evsn_pedal.idl`
- Legacy EMBD-v2-HighLevel PedalBox evidence from
  `configuration_pkg/config/bus.yml` and `5_PedalBox.eds`
- Legacy EMBD-v2-HighLevel physical architecture diagram:
  `../Sunswift/EMBD-v2-HighLevel/docs/physical_architecture.png`
- Legacy system launch:
  `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- Hardware-equivalence matrix:
  `.agents/exec_plans/evsn-network-port-roadmap.md`

Skills and references applied:

- `harness-creator`
- `qnx-cpp-rtos`
- `safety-critical-cpp`
- `cpp-integration-testing`
- `test-coverage-evidence`
- QNX process, scheduling, message/resource-manager, and tracing references
- Real-time C++ allocation, bounded-execution, and evidence references

## External Guidance

### QNX Documentation

QNX online documentation was consulted for this QNX-facing plan:

- QNX Product Documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- CAN `devctl()` command index:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can.html
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_rx_frame_raw_noblock.html
- `CAN_DEVCTL_TX_FRAME_RAW`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html
- `CAN_DEVCTL_SET_TIMING`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_set_timing.html
- C library `devctl()` reference:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/d/devctl.html
- C library `open()` reference:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/o/open.html

QNX design consequences:

- Use `evsn_can_hal` from `VSN-0019`; keep QNX CAN headers and driver-specific
  details private to the HAL backend.
- The first QNX backend should use raw `devctl()` CAN operations through QNX
  resource-manager endpoints where the selected driver supports raw frames.
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK` preserves bounded receive polling because
  an empty queue is reported as `EAGAIN`.
- `CAN_DEVCTL_TX_FRAME_RAW` transmit failure and transmit-queue-full behavior
  must be surfaced as explicit statuses, not hidden by unbounded retries.
- Official QNX raw CAN documentation describes classic CAN `struct can_msg`
  payloads with 8 data bytes. Do not assume CAN-FD behavior for the PedalBox
  path unless the selected driver/API proves it.
- Bitrate/timing changes with `CAN_DEVCTL_SET_TIMING` must be optional and
  deployment-controlled; a production target may own CAN timing outside this
  process.

### Open-Source CANopen Research

Open-source candidates reviewed:

- CANopenNode: https://github.com/CANopenNode/CANopenNode
- CANopenNode license: https://raw.githubusercontent.com/CANopenNode/CANopenNode/master/LICENSE
- Lely CANopen: https://opensource.lely.com/canopen/
- Lely CANopen overview:
  https://opensource.lely.com/canopen/docs/overview/
- Lely CANopen license:
  https://opensource.lely.com/canopen/docs/license/

First-pass recommendation:

- Prefer CANopenNode behind an EVSN-owned adapter.
- Keep Lely CANopen as the fallback candidate if CANopenNode cannot support the
  required master/client behavior cleanly after QNX cross-build evaluation.

Reasoning:

- CANopenNode is Apache-2.0, ANSI C, RTOS-oriented, and exposes a target driver
  boundary that can be implemented over `evsn_can_hal`.
- The PedalBox surface is small: NMT, heartbeat, TPDO receive, emergency
  handling, and optional SDO boot/config checks. That is a better fit for a
  bounded adapter than a large generic manager.
- Lely CANopen is also Apache-2.0 and capable, with a passive library design,
  but its dependency/event-system footprint needs QNX cross-build and
  allocation/threading audit before use in a safety-relevant input path.

### RTI Connext

RTI MCP guidance is not required for the pure CANopen decode logic, CAN HAL
adapter work, or local fake-CANopen-slave tests in this plan because those
steps do not create or change DDS entities, IDL, QoS, generated-code
integration, participants, readers, writers, listeners, or WaitSets.

RTI MCP guidance is required before:

- changing `EVSNPedalState` or adding a new DDS type,
- changing topic names or QoS,
- adding final DDS DataWriter/DataReader implementation,
- asserting deadline, liveliness, durability, or generated-code behavior.

## Selected CAN0 Drive-Control Bus Model

The legacy ROS2 launch/config is the compatibility source of truth for
unchanged hardware. `system_launch.xml` passes `can0` to the old
`pedal_box_driver`; the roadmap hardware-equivalence matrix also places BMS,
brake pressure, solar current, GNSS, motor telemetry, and final motor command
traffic on CAN0. Therefore `CAN0` is treated as a shared drive-control bus, not
a motor-only bus.

Do not infer the steerwheel bus from older diagrams in this PedalBox plan. The
old launch places the steerwheel driver on CAN3, and `VSN-0025` owns that
mapping.

Implementation option 2 is selected:

- Keep the PedalBox on CAN0.
- Create a separate QNX real-time CAN0 drive-control bus owner process,
  expected name `evsn_can0_drive_bus_rt`, that owns CAN0 HAL RX/TX.
- Keep motor-control computation in a separate motor-control process. It
  submits final motor command frames to the CAN0 drive-control bus owner through
  bounded QNX-local IPC, not DDS and not the generic gateway.
- Give final motor-command frame requests highest priority inside the CAN0
  drive-control bus owner.
- Do not route CAN0 PedalBox traffic through DDS or the generic CAN gateway for
  command/management TX.
- Do not allow the PedalBox manager, telemetry, cloud, or display processes to
  open a CAN0 TX endpoint directly.

The PedalBox path is receive-first. The plan should not assume writes to the
PedalBox unless the old CANopen `bus.yml`, `5_PedalBox.eds`, device
documentation, or bench evidence requires NMT, SDO, SYNC, heartbeat, or other
CANopen management frames.

The remaining design blocker is the CAN0 drive-control bus owner contract:

- owner process: `evsn_can0_drive_bus_rt`,
- priority, runmask, period, queue depth, and admission policy,
- whitelist for final motor command IDs and any verified CANopen management
  COB-IDs,
- rate limits and startup-only behavior for PedalBox NMT/SDO/SYNC if they are
  proven necessary,
- policy for suppressing non-essential CAN0 TX during queue pressure or
  timing-budget pressure,
- timing evidence proving final motor-command transmit deadlines are
  unaffected by PedalBox handling.

The expected implementation is that the PedalBox CANopen manager receives CAN0
raw frames from the CAN0 drive-control bus owner's read-only mirror. If
PedalBox CANopen management TX is required, the PedalBox manager submits
bounded, whitelisted requests to `evsn_can0_drive_bus_rt`; that process alone
performs CAN0 HAL writes.

## Proposed Architecture

### Package

Create a dedicated package:

- `evsn-runtime/canopen_pedalbox/CMakeLists.txt`
- `evsn-runtime/canopen_pedalbox/README.md`
- `evsn-runtime/canopen_pedalbox/include/evsn/canopen_pedalbox/*.hpp`
- `evsn-runtime/canopen_pedalbox/src/*.cpp`
- `evsn-runtime/canopen_pedalbox/src/canopennode_adapter/*.c`
- `evsn-runtime/canopen_pedalbox/test/*.cpp`
- `evsn-runtime/canopen_pedalbox/test/data/5_PedalBox.expected.yml`

Do not expose CANopenNode, Lely, QNX, or driver headers from public EVSN
headers. Public headers should expose EVSN-owned value types, result types, and
interfaces only.

### Runtime Shape

Preferred process: `evsn_canopen_pedalbox`

Threads:

- CAN RX worker:
  - Reads from the CAN0 drive-control bus owner's read-only mirror or from
    `evsn_can_hal` only when this package is built for a non-CAN0 test bus.
  - Accepts only configured CANopen COB-IDs for node ID `5`.
  - Feeds frames into the CANopen stack adapter.
- CANopen tick worker:
  - Runs a documented fixed period, expected first-pass period `1 ms`.
  - Processes CANopen timers, heartbeat supervision, NMT state, SDO boot checks
    if required, and emergency state.
  - Uses fixed-size storage initialized before entering steady state.
- Publisher/status worker:
  - Runs below motor-control priority.
  - Copies the latest validated `PedalBoxSample` into `EVSNPedalState`.
  - Publishes DDS only after the RTI gate clears.

When configured for CAN0, a TX request path also exists:

- It is disabled by default unless legacy configuration, EDS/DCF, device
  documentation, or bench evidence proves PedalBox management TX is required.
- If enabled, the PedalBox CANopen stack emits NMT/SDO/SYNC or other management
  frames.
- Adapter validates the frame against the CAN0 management whitelist and rate
  limits.
- Adapter submits a fixed-size request to `evsn_can0_drive_bus_rt`.
- `evsn_can0_drive_bus_rt` either transmits it in an allowed management slot or
  rejects it with an explicit diagnostic status.
- Final motor command frame requests always preempt or reject PedalBox
  management traffic.

The CANopen tick and RX design may be collapsed into one deterministic event
loop if the chosen stack proves a simpler bounded model. Any change must retain
bounded polling, bounded shutdown, explicit diagnostics, and no steady-state
dynamic allocation.

### Data Model

Internal types:

- `PedalBoxCanopenConfig`
  - bus name or HAL channel config
  - master node ID
  - PedalBox node ID
  - bitrate/timing policy
  - TPDO COB-ID
  - heartbeat timeout
  - stale-input timeout
  - scaling/range constants derived from EDS/DCF evidence
- `PedalBoxRawSample`
  - raw accelerator value
  - raw brake value
  - raw brake-switch value
  - source COB-ID
  - monotonic receive timestamp
  - sequence counter
- `PedalBoxState`
  - normalized accelerator percent/scalar
  - brake status
  - freshness
  - heartbeat/NMT state
  - emergency state
  - diagnostics bitfield
  - sample counter

The first implementation should generate or manually freeze EDS-derived
constants as reviewed source data. Runtime EDS parsing is allowed only in host
tools/tests, not in the QNX steady-state process.

### CANopen Behavior

Required minimum behavior:

- Initialize CANopen master/client state for master node ID `1`.
- Supervise PedalBox node ID `5`.
- Accept boot-up and heartbeat frames from `0x700 + 5`.
- Drive or verify NMT operational transition according to the final bus
  ownership decision.
- Receive TPDO `0x180 + 5 = 0x185`.
- Decode accelerator, brake, and brake-switch values according to
  EDS/DCF-derived mapping and byte order.
- Reject unexpected COB-IDs, invalid DLC, node-ID mismatch, mapping mismatch,
  malformed payloads, and stale samples.
- Capture emergency frames and expose a diagnostic fault bit.
- Enter stale/invalid input state when heartbeat or TPDO freshness expires.

Optional behavior, enabled only if the EDS/DCF and bench evidence require it:

- SDO upload at startup to verify identity, mapping, and heartbeat producer
  settings.
- SDO download to configure heartbeat or PDO parameters.
- SYNC production if the PedalBox TPDO mode requires it.

### CAN HAL Boundary

All CAN I/O goes through `evsn_can_hal`:

- The PedalBox package must not directly include `<sys/can_dcmd.h>`.
- QNX `devctl()` behavior remains inside `VSN-0019` HAL backends.
- Host tests use the fake HAL backend.
- QNX/SIL tests may use `dev-can-linux -L` through the QNX HAL backend when a
  target or QEMU contract is available.
- On production CAN0, HAL writes are owned only by `evsn_can0_drive_bus_rt`.
  The PedalBox package may use the HAL directly only for non-CAN0 test buses or
  isolated SIL configurations.

## Open-Source Stack Work Plan

### Phase 1: Stack Selection

- Pin candidate versions and licenses for CANopenNode and Lely CANopen.
- Record license compatibility, provenance, build system impact, and source
  import strategy.
- Cross-build the minimal selected stack for the EVSN QNX toolchain.
- Audit required modules, compile flags, heap use, static storage, threading,
  blocking calls, and error reporting.
- Choose one stack and record a design decision in this plan and the work log.

### Phase 2: CANopenNode First-Pass Adapter

If CANopenNode is selected:

- Import or vendor only the required modules after approval.
- Implement the CANopenNode CAN driver boundary over `evsn_can_hal`.
- Keep CANopen stack state in fixed storage initialized during process startup.
- Generate or hand-maintain a static object dictionary/config fragment for the
  PedalBox manager side as needed.
- Build a small PedalBox-specific adapter API:
  - `initialize(config)`
  - `process_tick(now)`
  - `submit_rx_frame(frame)`
  - `take_latest_sample(sample)`
  - `diagnostics()`
  - `shutdown()`

### Phase 3: Fallback Evaluation

If CANopenNode fails QNX, licensing, allocation, or master/client requirements:

- Evaluate Lely CANopen with a passive, HAL-fed event loop.
- Disable unneeded features and file/runtime EDS parsing in the target process.
- Repeat the cross-build, allocation, threading, and bounded-operation audit.

### Phase 4: Minimal In-House Subset

Implement a minimal in-house CANopen subset only if both open-source stacks are
rejected. The subset must be limited to the PedalBox requirements: NMT,
heartbeat consumer, TPDO receive, emergency receive, optional SDO boot checks,
and deterministic diagnostics.

## Subagents and Skills

Use subagents only for bounded assignments:

- `researcher`: inspect legacy PedalBox EDS/DCF/config and produce an object,
  COB-ID, scaling, heartbeat, and NMT mapping table. No file mutation.
- `researcher`: evaluate CANopenNode versus Lely for license, QNX build,
  module set, allocation behavior, and adapter surface. No file mutation.
- `implementer`: build the selected stack adapter and pure decode/state-machine
  logic. Edits limited to `evsn-runtime/canopen_pedalbox/` and assigned tests.
- `integration-tester`: build fake PedalBox slave tests and QNX/SIL smoke
  commands. No production behavior changes.
- `reviewer`: review CAN0 ownership, QNX docs evidence, bounded execution,
  allocation, stale behavior, and strict tests.

Required skills:

- `qnx-cpp-rtos`
- `qnx-realtime-control-loop`
- `qnx-cpp-performance`
- `safety-critical-cpp`
- `cpp-unit-testing`
- `cpp-integration-testing`
- `middleware-testing`
- `fault-injection-resilience-testing`
- `test-coverage-evidence`
- `rti-connext-dds` for DDS publication/type work after the RTI gate clears

## Strict Verification Gate

This work item must link evidence to `VSN-0033`. Do not mark it complete on a
successful build or unit tests alone.

Minimum required evidence:

- Smoke tests:
  - config validation for bus, node IDs, COB-ID, heartbeat, and stale timeout
  - startup without hardware using fake HAL
  - bounded shutdown from each runtime state
  - no direct CAN0 TX endpoint opened by the PedalBox process
  - PedalBox TX path disabled unless required by recorded legacy/device
    evidence
  - bounded CAN0 drive-control bus owner request rejection when motor traffic
    has priority
- Unit tests:
  - EDS-derived mapping constants for `0x2000`, `0x2001`, and `0x2002`
  - TPDO `0x185` decode with valid vectors
  - invalid DLC, unexpected COB-ID, node-ID mismatch, endian mismatch, and
    mapping mismatch
  - accelerator and brake range checks
  - brake-switch normalization
  - heartbeat timeout, TPDO stale timeout, boot-up handling, NMT states, and
    emergency-frame diagnostics
  - transmit-queue-full and receive-empty statuses from fake HAL
  - no dynamic allocation after initialization in the steady-state path where
    instrumentation is available
- Integration tests:
  - fake CANopen PedalBox slave exercising boot-up, heartbeat, TPDO, emergency,
    stale input, malformed payload, and shutdown
  - fake HAL to `EVSNPedalState` output pipeline without DDS
  - fake CAN0 drive-control bus owner arbitration test proving final motor
    command frame requests preempt or reject optional PedalBox management TX
  - DDS isolated-domain publication after RTI MCP guidance clears
  - QNX/SIL `dev-can-linux -L` raw-frame smoke when target/QEMU access exists
- Regression tests:
  - `VSN-0033` network regression includes the PedalBox manager
  - hardware-equivalence regression proves legacy TPDO `0x185` on CAN0
    produces the same accelerator normalization and disconnect semantics as
    the old simple driver for approved vectors
  - hardware-equivalence regression proves PedalBox management TX is absent by
    default and, if later enabled, uses only the `evsn_can0_drive_bus_rt`
    whitelist
  - scan rejects ROS2 `canopen_core` dependencies in production code
  - scan rejects retired `/car/...` topics and banned branding
  - negative regression proves the PedalBox manager cannot open CAN0 TX
    directly and cannot enqueue final motor-command IDs

Every evidence record must include command, platform, target, backend, selected
CANopen stack version, inputs, outputs/logs, exit status, residual risks, and
known gaps.

## Implementation Steps

- [ ] Define the `evsn_can0_drive_bus_rt` contract for implementation option 2.
- [ ] Confirm whether the PedalBox needs any CANopen management TX beyond
      receive-only TPDO/heartbeat handling.
- [ ] Add fixed-size CAN0 drive-control bus owner request and rejection
      diagnostics for optional PedalBox CANopen management TX.
- [ ] Freeze the PedalBox EDS/DCF-derived mapping table and scaling/range
      assumptions.
- [ ] Add hardware-equivalence vectors for legacy `0x185` accelerator
      normalization and CAN0 bus assignment.
- [ ] Select CANopenNode, Lely, or minimal in-house subset with license,
      version, provenance, QNX build, and allocation evidence.
- [ ] Define `PedalBoxCanopenConfig`, `PedalBoxRawSample`, and
      `PedalBoxState` pure types.
- [ ] Implement pure TPDO decode, state/freshness logic, and diagnostics.
- [ ] Implement the selected CANopen stack adapter over `evsn_can_hal`.
- [ ] Add fake HAL and fake CANopen PedalBox slave integration tests.
- [ ] Add QNX/SIL smoke using the HAL QNX backend after target/QEMU access is
      approved.
- [ ] Add DDS publication only after RTI MCP guidance is recorded.
- [ ] Wire package evidence into `VSN-0033`.
- [ ] Update `VSN-0025` so generic pedal input work delegates CANopen PedalBox
      behavior to this work item.

## Verification

- [ ] Command: CANopen stack provenance/license/build audit
      Result:
- [ ] Command: PedalBox pure unit tests
      Result:
- [ ] Command: fake HAL and fake CANopen PedalBox integration tests
      Result:
- [ ] Command: hardware-equivalence PedalBox `0x185` regression
      Result:
- [ ] Command: DDS isolated-domain publication test after RTI gate
      Result:
- [ ] Command: QNX/SIL `dev-can-linux -L` smoke when target/QEMU exists
      Result:
- [ ] Command: `./init.sh check`
      Result:
- [ ] Command: `git diff --check`
      Result:

## Blockers

- Implementation option 2 is selected, but the `evsn_can0_drive_bus_rt`
  contract, priorities, runmask, queue depth, whitelist, rate limits, and timing
  budget must be approved before code implementation.
- Confirm the physical PedalBox CANopen protocol, EDS completeness, object byte
  order, scaling, heartbeat behavior, and whether any NMT/SDO/SYNC write is
  required.
- Choose and approve the CANopen stack import strategy.
- Confirm QNX target or QEMU access for `dev-can-linux -L` or physical CAN
  smoke evidence.
- RTI Connext MCP guidance is required before final DDS publication/type/QoS
  implementation.

## Handoff

`VSN-0035` narrows the PedalBox part of `VSN-0025`. Steerwheel input remains in
`VSN-0025`; CANopen PedalBox ownership, boot/liveness, TPDO decode, and
PedalBox-specific tests move here.
