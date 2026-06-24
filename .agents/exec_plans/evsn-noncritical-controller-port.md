# VSN-0030 - Non-Critical Body Controller Port

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port `non_critical_controller` as a generic EVSN body-control controller for
lighting, indicators, headlight command state, taillight command state, and
other non-critical body outputs that still exist on the vehicle.

## Non-Goals

- Do not port horn behavior because the horn driver/system is ignored.
- Do not port firmware update action-client behavior.
- Do not preserve old branded package or topic names.
- Do not put body-control work on the motor-control real-time thread/core.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-display-lighting-drivers-port.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_logic/non_critical_controller/`

## External Guidance

- RTI MCP: unavailable. DDS type/topic/QoS implementation is blocked.
- QNX online docs: use roadmap QNX docs for runtime behavior.

## Proposed Architecture

- Consume typed steerwheel, brake pressure, regen, gear, and hazard inputs.
- Publish typed lighting command state for headlights, indicators, taillights,
  reverse, and brake-light requests.
- Implement bounded timers for indicator flashing and repeated taillight
  publish policy.
- Remove horn and firmware-update code paths from the port.
- Preserve old behavior as request/state mapping only: old outputs
  `/control/headlight/*`, `/control/taillights/*`, and `/control/indicator/*`
  become one typed generic lighting command contract that downstream CAN1/CAN2
  drivers convert into their legacy frames.
- The roadmap hardware-equivalence matrix owns the downstream frame checks:
  CAN1 indicator `0x18D`/`0x18E`, CAN1 taillight `0x18B`, and CAN2 headlight
  frames.

## Subagents and Skills

- `researcher`: map old body-control inputs, timers, and outputs.
- `implementer`: port pure body-control logic, DDS adapter, and tests.
- `reviewer`: verify ignored horn/firmware paths are absent, timing is bounded,
  and tests are present.
- Required skills: `rti-connext-dds`, `middleware-testing`,
  `cpp-unit-testing`, `cpp-integration-testing`, `test-coverage-evidence`,
  `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on state-machine unit tests alone.

Minimum evidence:

- smoke test for body controller startup with fake snapshots and bounded
  shutdown
- unit tests for lighting, indicators, headlights, taillights, brake lights,
  reverse, hazard behavior, stale inputs, contradictory inputs, and diagnostics
- hardware-equivalence regression for old body-control inputs to generic
  lighting command state, then downstream frame compatibility through
  `VSN-0026`
- DDS integration tests with real RTI entities after the RTI gate clears
- regression tests proving horn, firmware-update, centre-display, old
  USB/serial display output, and motor critical-path ownership are absent
- exact commands, input vectors, middleware domain/transport, logs, and gaps
  recorded before status changes

## Steps

- [ ] Define body-control IDL under `VSN-0021`.
- [ ] Rewrite lighting and indicator state machines as pure C++.
- [ ] Remove horn and firmware update paths.
- [ ] Add golden old-to-new body-control vectors for lighting, taillights,
      indicators, hazards, and brake/reverse behavior.
- [ ] Add DDS adapter after RTI MCP guidance is recorded.
- [ ] Add integration tests with headlight, indicator, and taillight fake
      subscribers.

## Verification

- [ ] Smoke: controller validates config and exits boundedly.
- [ ] Unit: brake threshold, reverse/gear, indicator flash, hazard behavior,
      stale input, and repeated publish policy.
- [ ] Regression: hardware-equivalence body-control command vectors.
- [ ] Integration: DDS input state to lighting command output.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0025`, `VSN-0026`, `VSN-0027`, and RTI MCP
guidance.
