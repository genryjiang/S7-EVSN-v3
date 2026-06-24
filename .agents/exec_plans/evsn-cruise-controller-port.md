# VSN-0029 - Cruise Controller Port

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port `cruise_control` into an EVSN controller package that publishes typed
cruise request/status snapshots for the drive controller. It must be generic,
bounded, and separate from final motor command output.

## Non-Goals

- Do not send motor commands.
- Do not keep BWSC-specific comments or parameter names in new contracts.
- Do not use unbounded PID or smoothing state.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-drive-controller-port.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_logic/cruise_control/`

## External Guidance

- RTI MCP: unavailable. DDS request/status implementation is blocked.
- QNX online docs: use roadmap QNX docs for timer/runtime behavior.

## Proposed Architecture

- Pure cruise controller consumes steerwheel cruise buttons, brake status,
  pedal state, vehicle speed, gear, and parameter snapshots.
- Output typed cruise state with target speed, requested accel, requested regen,
  requested gear, enable state, disabling reason, timestamp, and diagnostics.
- Use explicit parameter bounds for PID constants, max accel, max speed, and
  regen constants.
- Stale or contradictory inputs disable cruise and publish a reason.
- Preserve old behavior as an advisory request: old outputs `/cruise/state`,
  `/cruise/target_kmh`, `/cruise/accel`, `/cruise/regen`, `/cruise/gear`, and
  `/cruise/disabling_event` become one generic typed cruise state consumed by
  the drive controller. Cruise never owns final CAN0 motor command TX.

## Subagents and Skills

- `researcher`: map old cruise subscriptions, parameters, and disable paths.
- `implementer`: port pure cruise logic, DDS/parameter adapter, and tests.
- `reviewer`: verify bounded control behavior, stale disable, and evidence.
- Required skills: `rti-connext-dds`, `cpp-unit-testing`,
  `cpp-integration-testing`, `middleware-testing`, `test-coverage-evidence`,
  `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on PID or smoothing unit tests alone.

Minimum evidence:

- smoke test for controller startup with fake snapshots and bounded shutdown
- unit tests for setpoint changes, PID/smoothing limits, brake disable, gear
  disable, stale input disable, contradictory inputs, and boundary values
- hardware-equivalence regression using old steerwheel cruise, brake,
  accelerator, motor speed, and gear vectors, with expected typed cruise
  request/status output
- DDS integration tests with real RTI entities after the RTI gate clears
- regression tests proving BWSC-specific names/comments are removed and cruise
  output cannot own final CAN0 motor commands
- exact commands, input vectors, middleware domain/transport, logs, and gaps
  recorded before status changes

## Steps

- [ ] Define typed cruise IDL under `VSN-0021`.
- [ ] Rewrite PID/smoothing logic as pure bounded C++.
- [ ] Add parameter descriptors and fallback behavior.
- [ ] Add golden old-to-new cruise vectors for target changes, brake disable,
      gear disable, throttle override, and stale speed.
- [ ] Add DDS adapter after RTI MCP guidance is recorded.
- [ ] Add integration test with fake steerwheel, brake, speed, and gear inputs.

## Verification

- [ ] Smoke: controller validates config and exits boundedly.
- [ ] Unit: enable/disable, target up/down, PID boundaries, stale input,
      brake disable, and limit behavior.
- [ ] Regression: hardware-equivalence cruise request/status vectors.
- [ ] Integration: DDS inputs to typed cruise status/request output.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0025`, `VSN-0028`, and RTI MCP guidance.
