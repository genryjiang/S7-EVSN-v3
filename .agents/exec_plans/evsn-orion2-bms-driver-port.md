# VSN-0024 - Orion 2 BMS Driver Port

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port `orion2_bms_driver` from ROS2 to an EVSN RTI/QNX package. Keep BMS naming
and semantics, but replace ROS2 topics and messages with typed EVSN IDL.

Old behavior:

- subscribe `/can0/from_can_bus`
- decode standard CAN IDs `0x620` through `0x629`
- publish pack, cell, thermistor, relay, precharge, and DTC values

## Non-Goals

- Do not port Lithbal or Elithion BMS logic.
- Do not route motor commands through this driver.
- Do not publish one scalar DDS topic per old ROS2 scalar unless the contract
  review explicitly chooses that shape.

## Context Loaded

- `.agents/exec_plans/evsn-network-contracts-idl.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/orion2_bms_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/bms_msgs/msg/*.msg`

## External Guidance

- RTI MCP: unavailable. DDS type/topic/QoS implementation is blocked.
- QNX online docs: use roadmap QNX docs for runtime, scheduling, and build
  behavior.

## Subagents and Skills

- `researcher`: map every old CAN ID and output field to a typed BMS contract.
- `implementer`: port only BMS decode logic, DDS adapter, docs, and tests.
- `reviewer`: check CAN ID filtering, units, stale behavior, and tests.
- Required skills: `rti-connext-dds`, `middleware-testing`,
  `cpp-unit-testing`, `cpp-integration-testing`, `test-coverage-evidence`,
  `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on decode unit tests alone.

Minimum evidence:

- smoke test for driver startup, fake CAN input, DDS output readiness, and
  bounded shutdown
- unit tests for CAN ID filtering, DLC checks, payload bounds, scaling,
  validity, freshness, diagnostics, and malformed inputs
- hardware-equivalence regression for all ten legacy BMS frames
  `0x620..0x629` received from the CAN0 drive-control bus owner mirror
- DDS integration tests with real RTI entities after the RTI gate clears
- regression tests for stale BMS frames, missing frames, fault/DTC propagation,
  and no Lithbal or Elithion/Lithiumate path reintroduction
- exact commands, middleware domain/transport, inputs, outputs, logs, and gaps
  recorded before status changes

## Steps

- [ ] Convert BMS ROS2 messages to EVSN BMS IDL under `VSN-0021`.
- [ ] Rewrite decode as pure, fixed-input logic with explicit length checks.
- [ ] Reject extended frames and non-`0x620..0x629` IDs deterministically.
- [ ] Preserve the roadmap hardware-equivalence mapping from old
      `/car/bms/...` topics to generic `/vehicle/bms/...` candidate DDS
      contracts.
- [ ] Add DDS adapter for raw CAN input and typed BMS output after RTI gate.
- [ ] Add stale-input diagnostics and sample counters.
- [ ] Add README with CAN IDs, units, topics, and evidence.

## Verification

- [ ] Smoke: node starts with fake DDS/CAN config and exits cleanly.
- [ ] Unit: decode all `0x620..0x629` payloads, boundaries, malformed DLC, and
      ignored IDs.
- [ ] Regression: hardware-equivalence BMS frame set from CAN0 mirror.
- [ ] Integration: publish raw CAN frames on isolated DDS domain and assert
      typed BMS outputs.
- [ ] QNX target: run bounded startup/shutdown smoke when target is available.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0022`, and RTI MCP guidance.
