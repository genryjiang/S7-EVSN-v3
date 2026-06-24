# VSN-0021 - EVSN Network Contracts and IDL

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Convert active ROS2 message definitions and implicit topic contracts from
EMBD-v2-HighLevel into EVSN IDL, generated RTI Connext C++ types, and a generic
topic registry.

Inputs:

- `vehicle_interface/can_msgs/msg/Frame.msg`
- `vehicle_interface/bms_msgs/msg/*.msg`
- `vehicle_interface/cmu_msgs/msg/*.msg`
- topic names discovered in active drivers/controllers/telemetry packages

Existing EVSN types:

- `EVSNCan20Frame`
- `EVSNFDCanFrame`
- `EVSNPedalState`
- `EVSNMotor*`
- `EVSNDrive*`
- `EVSNParameter*`

## Non-Goals

- Do not implement node behavior in this work item.
- Do not generate final RTI code until RTI MCP guidance is recorded.
- Do not keep ROS2 `std_msgs/Header` as an IDL dependency; use explicit EVSN
  timestamps, sample counters, freshness flags, and source identifiers.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix
- `evsn-runtime/dds_types/std_types/evsn_can20/evsn_can20.idl`
- `evsn-runtime/dds_types/std_types/evsn_fdcan/evsn_fdcan.idl`
- `evsn-runtime/dds_types/custom_types/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/can_msgs/msg/Frame.msg`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/bms_msgs/msg/*.msg`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/cmu_msgs/msg/*.msg`

## External Guidance

- RTI MCP: unavailable in this session. Final IDL, QoS, keying, durability,
  generated-code integration, and compatibility decisions are blocked until
  RTI Connext MCP evidence is recorded.
- QNX online docs: QNX docs are indirectly relevant for timestamp and runtime
  behavior; see `VSN-0020` for consulted URLs.

## Contract Plan

- Keep raw CAN as `EVSNCan20Frame` for classic buses unless FD metadata is
  required. `EVSNFDCanFrame` currently lacks explicit DLC, extended-ID, and RTR
  metadata, so use it only after RTI-guided contract review.
- The old ROS2 topic names in the roadmap hardware-equivalence matrix are
  compatibility references, not public EVSN topic names. The type/topic
  registry must map every old active topic to a generic EVSN candidate topic or
  to an explicit retired/excluded status.
- The registry must include bus, CAN ID, direction, old topic/source, new owner
  process, candidate DDS topic, and required regression ID for every matrix row
  before dependent package implementation begins.
- Add BMS IDL types equivalent to:
  - `Ccl`, `Dcl`, `Celldata`, `Thermistor`, `Hightemp`, `Lowtemp`, `Relay`,
    `Precharge`, `Dtc1`, and `Dtc2`.
- Add CMU IDL types equivalent to `Cmut` and `Cmuv` only if the migrated
  topology still has a CMU data source.
- Replace one-field ROS2 scalar topics with named EVSN typed structures when
  the value is part of a coherent domain contract. Examples:
  - driver input states
  - GNSS/navigation state
  - vehicle body-control state
  - lighting command state
  - energy telemetry snapshots
- Centre display and the old USB/serial `display_driver` output path must be
  recorded as excluded MVP registry entries, not generated runtime DDS
  contracts. Future HDMI Qt driver display snapshot contracts belong to
  deferred `VSN-0037` and require RTI guidance before implementation.
- Define generic topic names. Examples:
  - `/vehicle/bms/...` remains BMS-specific.
  - `/vehicle/input/pedal/state`
  - `/vehicle/input/steerwheel/state`
  - `/vehicle/navigation/gnss/state`
  - `/vehicle/body/lighting/command`
  - `/vehicle/body/lighting/status`
  - `/vehicle/sensors/...`
  - `/vehicle/energy/...`
  - `/vehicle/cloud/...`
- Remove branded topic fragments such as `/bwsc`, `sunswift7`, `SR`, and
  BWSC-specific labels. Keep BMS labels.

## Subagents and Skills

- `researcher`: enumerate exact old ROS2 topics and message fields per package.
- `implementer`: edit only IDL, generated-type CMake, and contract docs.
- `reviewer`: verify RTI MCP evidence, field units/ranges, compatibility, and
  test coverage.
- Required skills: `rti-connext-dds`, `middleware-testing`,
  `test-coverage-evidence`, `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on IDL generation alone.

Minimum evidence:

- IDL generation/build and serialization tests for every active bounded type
- topic registry tests for units, ranges, freshness fields, and stale-data
  semantics
- hardware-equivalence registry tests proving every active launch/config
  CAN frame and old ROS2 topic in `VSN-0020` has a new owner, candidate DDS
  topic or explicit "no DDS" decision, and regression ID
- compatibility tests that detect renamed/retired ROS2 topics, banned branded
  fragments, and schema drift
- DDS integration tests with real RTI entities on isolated domains after the
  RTI gate clears, including QoS/deadline/liveliness expectations and negative
  mismatch cases where practical
- exact commands, platform, generated-code inputs/outputs, and residual gaps
  recorded in the active log

## Steps

- [ ] Build an old-to-new topic and type registry.
- [ ] Add hardware-equivalence matrix fields to the registry: bus, frame ID,
      direction, old ROS2 topic/source, new QNX process, candidate DDS topic,
      and regression ID.
- [ ] Decide which existing EVSN IDL types can be reused without semantic loss.
- [ ] Draft BMS and CMU IDL with field units, ranges, validity, timestamps, and
      stale-data policy.
- [ ] Run RTI MCP and record guidance for IDL bounded sequences, key fields,
      QoS, and generated-code integration.
- [ ] Generate RTI C++ types after guidance is recorded.
- [ ] Add type serialization/unit tests and IDL compatibility tests.
- [ ] Update every dependent package plan with final type names.

## Verification

- [ ] Command: `./init.sh check`
      Result:
- [ ] Command: `git diff --check`
      Result:
- [ ] Command: focused IDL generation/build command
      Result:
- [ ] Command: focused CTest for new IDL serialization and compatibility
      Result:
- [ ] Command: hardware-equivalence topic/type registry regression
      Result:

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Implementation is blocked on RTI Connext MCP guidance.
