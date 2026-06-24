# VSN-0028 - Drive Controller Port

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port `drive_controller` into an EVSN controller package that computes driver
request and limit state for the QNX motor-control critical path.

The controller may publish desired drive state or request snapshots over DDS,
but it must not send motor CAN frames and must not own CAN0.

## Non-Goals

- Do not port remote-driving control paths.
- Do not publish final motor commands.
- Do not use a telemetry or cloud path as a control input.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-motor-control-critical-path.md`
- `.agents/exec_plans/evsn-gps-can-driver-port.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix
- `evsn-runtime/dds_types/custom_types/evsn_drive/evsn_drive.idl`
- `evsn-runtime/dds_types/custom_types/evsn_motor/evsn_motor.idl`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_logic/drive_controller/`

## External Guidance

- RTI MCP: unavailable. DDS request/status implementation is blocked.
- QNX online docs: use roadmap QNX docs for runtime behavior.

## Proposed Architecture

- Pure logic computes accelerator, regen, speed, gear, mode, BMS CCL, cruise
  and local limit effects into a bounded drive request.
- GPS speed comes from the typed GNSS/navigation state produced by `VSN-0036`,
  not the legacy `/car/speed/gps` scalar topic.
- Remote-driving subscriptions are removed.
- Dynamic parameters become EVSN parameter service descriptors with units,
  bounds, mutability, and fallback behavior.
- Motor-control process consumes copied snapshots and independently computes
  final command and safe state.
- For unchanged hardware, golden compatibility vectors must preserve the old
  drive-controller behavior for manual/cruise inputs: the old outputs
  `/control/motors/current` and `/control/motors/velocity` become request
  snapshot fields, then `evsn_motor_control_rt` converts the final approved
  request into the WS22 `0x501` frame. The drive controller itself never owns
  CAN0 TX.
- The old `/control/steering/angle` output is explicitly retired because the
  steering ECU/EPAS actuator motor has been removed. Drive-controller
  equivalence must prove the steering output path is absent and that no
  replacement steering actuator command is emitted.

## Subagents and Skills

- `researcher`: map old inputs, outputs, limits, and disabled remote paths.
- `implementer`: port pure drive logic, DDS/parameter adapter, and tests.
- `reviewer`: verify no direct CAN0 command path, stale behavior, and tests.
- Required skills: `rti-connext-dds`, `qnx-realtime-control-loop`,
  `middleware-testing`, `cpp-unit-testing`, `cpp-integration-testing`,
  `test-coverage-evidence`, `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on controller unit tests alone.

Minimum evidence:

- smoke test for controller startup with fake snapshots and bounded shutdown
- unit tests for drive limits, gear modes, BMS CCL/DCL, cruise arbitration,
  stale inputs, contradictory inputs, disables, boundary values, and safe
  degraded outputs
- hardware-equivalence regression using old ROS2 input vectors for
  `/car/accel_pos`, `/car/regen_pos`, `/car/speed/gps`,
  `/car/velocity_kph/right`, `/car/steerwheel/{drive_mode,gear}`, `/cruise/*`,
  and `/car/bms/pack/ccl`; expected outputs are request/status snapshots, not
  direct CAN frames, and the retired `/control/steering/angle` output is absent
- DDS integration tests with real RTI entities after the RTI gate clears
- regression tests proving remote-driving paths are absent and outputs are
  request/status snapshots, not final CAN0 motor commands
- exact commands, input vectors, middleware domain/transport, logs, and gaps
  recorded before status changes

## Steps

- [ ] Define typed drive request/status contract under `VSN-0021`.
- [ ] Rewrite limit and mode logic as pure deterministic C++.
- [ ] Remove remote-driving code paths.
- [ ] Add parameter descriptors for limits and fallback behavior.
- [ ] Add golden old-to-new request-vector tests for manual and cruise drive
      scenarios.
- [ ] Replace old GPS speed scalar input with the typed GNSS speed/freshness
      contract from `VSN-0036` after RTI guidance.
- [ ] Add DDS adapter after RTI MCP guidance is recorded.
- [ ] Add integration test with fake pedal, steerwheel, BMS, cruise, and motor
      control snapshot consumer.

## Verification

- [ ] Smoke: controller starts with fake inputs and exits boundedly.
- [ ] Unit: limits, gear/mode transitions, stale input, BMS CCL, cruise
      arbitration, and invalid values.
- [ ] Regression: hardware-equivalence drive-controller request vectors.
- [ ] Integration: DDS input snapshots to typed drive request/status output.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0023`, `VSN-0024`, `VSN-0025`, `VSN-0029`, and RTI
MCP guidance.
