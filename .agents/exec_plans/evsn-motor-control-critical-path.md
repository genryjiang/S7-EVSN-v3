# VSN-0023 - QNX Motor-Control Critical Path

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Rework motor control as a QNX real-time critical path instead of a DDS-routed
node command path, while using a separate real-time CAN0 drive-control bus
owner process for CAN0 HAL writes.

Ownership model:

- CAN0 is the drive-control bus, not a motor-only bus. The
  EMBD-v2-HighLevel launch/config plus active driver constants are the
  compatibility baseline for unchanged hardware. See the hardware-equivalence
  matrix in `.agents/exec_plans/evsn-network-port-roadmap.md`.
- `evsn_motor_control_rt` owns final motor command computation.
- A separate QNX real-time process, expected name `evsn_can0_drive_bus_rt`,
  owns CAN0 HAL RX/TX access and writes frames to CAN0.
- Final motor command frames flow from `evsn_motor_control_rt` to
  `evsn_can0_drive_bus_rt` through bounded QNX-local IPC, not DDS and not the
  generic gateway.
- `VSN-0035` selected CANopen PedalBox implementation option 2: PedalBox
  remains on CAN0, but PedalBox management TX is disabled unless legacy/device
  evidence requires it. If required, it must go through
  `evsn_can0_drive_bus_rt`, never through direct PedalBox TX.
- Do not route CAN0 motor commands through DDS.
- Do not route CAN0 motor commands through a generic CAN gateway node.
- Do not make telemetry responsible for sending motor commands.

The process may publish copied motor-control status and motor telemetry to DDS
for non-critical consumers, but DDS must not back-pressure or influence the
hot path.

Legacy motor protocol to preserve for unchanged hardware:

- drive command: standard CAN ID `0x501`
- optional power command: standard CAN ID `0x502`
- optional reset command: standard CAN ID `0x503`
- left telemetry: standard CAN IDs `0x400..0x417`
- right telemetry: standard CAN IDs `0x420..0x437`
- old request topics: `/control/motors/current`, `/control/motors/velocity`,
  and `/car/motors/reset`
- old current scaling: request `0..100` maps to WS22 command current `0..1`
  after clamping
- old payload order on the existing host path: velocity float bytes in
  `data[0..3]`, current float bytes in `data[4..7]`

## Non-Goals

- Do not reuse the old `can_broker` motor command path.
- Do not implement remote-driving command support.
- Do not put logging, cloud telemetry, filesystem, database, or DDS writes in
  the real-time loop.
- Do not claim safety compliance without lifecycle evidence.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-can-broker-adaptation.md`
- `.agents/exec_plans/evsn-can-abstraction-layer.md`
- `.agents/exec_plans/evsn-canopen-pedalbox-qnx-port.md`
- `../Sunswift/EMBD-v2-HighLevel/docs/physical_architecture.png`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `evsn-runtime/dds_types/custom_types/evsn_motor/evsn_motor.idl`
- `evsn-runtime/dds_types/custom_types/evsn_drive/evsn_drive.idl`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/can_broker/src/motor_driver_ws22.cpp`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_logic/drive_controller/`

## External Guidance

- RTI MCP: unavailable. DDS status/telemetry contracts for this process remain
  blocked until RTI guidance is recorded, but pure QNX real-time loop planning
  may proceed using QNX online docs.
- QNX online docs:
  - `clock_nanosleep()` for absolute-time release.
  - `pthread_setschedparam()` and scheduling policy docs for `SCHED_FIFO`.
  - `ThreadCtl()` and processor affinity docs for runmask setup.
  - `tracelogger` and TraceEvent docs for timing evidence.

## Proposed Architecture

- Process: `evsn_motor_control_rt` for final motor command computation.
- Process: `evsn_can0_drive_bus_rt` for CAN0 drive-control bus ownership and
  HAL writes plus bounded receive fanout to CAN0 subscribers.
- Hot thread: one `SCHED_FIFO` real-time thread with documented priority,
  runmask, period, deadline, jitter budget, and WCET budget.
- Input receivers: lower-priority DDS/CAN/parameter receiver threads update
  atomic or double-buffered snapshots.
- Output path: preallocated final motor command frame handed to
  `evsn_can0_drive_bus_rt` through bounded QNX-local IPC with checked failure
  handling.
- CAN0 drive-control bus owner path: fixed-size priority queues, with final
  motor command requests highest priority and optional `VSN-0035` PedalBox
  CANopen management requests accepted only if they are proven necessary,
  whitelisted, rate-limited, and allowed to reject under pressure.
- CAN0 receive fanout: deliver raw CAN0 frames to PedalBox, BMS, brake
  pressure, solar current, GPS/GNSS, motor telemetry, and optional tyre
  pressure owners according to the roadmap matrix; no receive subscriber gets a
  direct CAN0 TX endpoint.
- Telemetry: copied snapshots handed to a lower-priority DDS publisher outside
  the loop.
- Safe state: stale inputs, invalid commands, missed deadlines, transmit
  failure, or contradictory state force neutral/standby command according to
  the approved motor safety concept.

## Subagents and Skills

- `researcher`: inspect current motor, drive, pedal, and CAN contracts.
- `implementer`: edit only motor critical-path package and assigned tests.
- `reviewer`: audit timing, allocation, blocking, QNX docs, and evidence.
- `target-tester`: run QNX QEMU/target loop timing smoke when approved.
- Required skills: `qnx-realtime-control-loop`, `qnx-cpp-rtos`,
  `qnx-cpp-performance`, `safety-critical-cpp`,
  `fault-injection-resilience-testing`, `cpp-unit-testing`,
  `cpp-integration-testing`, `test-coverage-evidence`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on pure control unit tests alone.

Minimum evidence:

- smoke tests for process startup, configuration validation, safe initial
  output, and bounded shutdown
- unit tests for safe state, stale snapshots, contradictory inputs, limit
  boundaries, transmit failure, deadline miss accounting, and no-allocation
  steady-state behavior
- golden compatibility tests for the legacy WS22 `0x501` drive frame, optional
  `0x502`/`0x503` service frames, and `0x400..0x417`/`0x420..0x437` telemetry
  fanout
- integration tests for fake CAN0 transmit and copied DDS status/telemetry
  adapters, with DDS strictly outside the hot path after the RTI gate clears
- integration tests for the bounded `evsn_motor_control_rt` to
  `evsn_can0_drive_bus_rt` command handoff
- integration tests for CAN0 receive fanout covering PedalBox `0x185`, BMS
  `0x620..0x629`, brake pressure `0x1B3`, solar current `0x1B4`, GNSS
  `0x700..0x704`, and motor telemetry IDs
- QNX timing regression with period, compute time, transmit time, jitter, and
  deadline-miss evidence captured with `tracelogger` when target access exists
- negative regression proving no DDS topic, generic gateway, telemetry, cloud,
  database, or filesystem path can own final CAN0 motor command TX

## Steps

- [ ] Define motor-control requirements: period, deadline, jitter budget,
      priority, runmask, stale thresholds, and safe-state outputs.
- [ ] Define `evsn_can0_drive_bus_rt` ownership, priority, runmask, queue
      depth, admission policy, and direct QNX-local IPC contract from
      `evsn_motor_control_rt`.
- [ ] Define final WS22 motor command frame ownership for `0x501` and
      explicitly block generic CAN0 TX.
- [ ] Define CAN0 receive fanout and subscriber admission using the roadmap
      hardware-equivalence matrix.
- [ ] Define the optional `VSN-0035` PedalBox CANopen management request
      whitelist, queue depth, rate limit, rejection policy, and timing budget,
      only if PedalBox writes are required by legacy/device evidence.
- [ ] Build pure motor-control decision logic with fixed-size snapshots.
- [ ] Add QNX real-time loop using absolute-time scheduling.
- [ ] Add no-allocation instrumentation for jitter, compute time, transmit
      time, period, and deadline misses.
- [ ] Add DDS telemetry/status adapter outside the hot path after RTI MCP
      guidance is recorded.
- [ ] Add smoke, unit, integration, fault-injection, and QNX target timing tests.

## Verification

- [ ] Command: pure logic unit tests
      Result:
- [ ] Command: fake CAN0 transmit failure and stale-input tests
      Result:
- [ ] Command: hardware-equivalence motor/CAN0 fanout regression
      Result:
- [ ] Command: isolated DDS status integration tests after RTI gate
      Result:
- [ ] Command: QNX target timing smoke with `tracelogger`
      Result:
- [ ] Command: `./init.sh check`
      Result:

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

This work item supersedes the older DDS motor-driver command-routing
assumption. Implementation can begin only after the control-period,
`evsn_can0_drive_bus_rt` ownership, target priority/runmask, and bounded IPC
requirements are approved.
