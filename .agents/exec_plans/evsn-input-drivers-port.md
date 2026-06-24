# VSN-0025 - Pedal and Steerwheel Driver Ports

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port active driver logic from:

- `pedal_box_driver`
- `steerwheel_driver`

Both become EVSN input-driver packages using typed DDS contracts and local
CAN-ID decode logic.

## Non-Goals

- Do not port remote-driving input paths.
- Do not publish old `/car/...` topic names in the new runtime.
- Do not put either driver in the CAN0 motor command hot path. The pedal driver
  publishes input state only; the motor-control process consumes a copied
  snapshot.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-motor-control-critical-path.md`
- `evsn-runtime/dds_types/custom_types/evsn_pedal/evsn_pedal.idl`
- `evsn-runtime/dds_types/custom_types/evsn_drive/evsn_drive.idl`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/pedal_box_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/steerwheel_driver/`

## External Guidance

- RTI MCP: unavailable. DDS type/topic/QoS implementation is blocked.
- QNX online docs: use roadmap QNX docs for runtime and scheduling behavior.

## Pedal Driver Plan

The CANopen PedalBox path is split into `VSN-0035`:
`.agents/exec_plans/evsn-canopen-pedalbox-qnx-port.md`. This generic input
driver plan should not treat the PedalBox as a simple raw-CAN scalar decoder
unless `VSN-0035` explicitly rejects CANopen stack integration.

- Preserve the old launch assignment: PedalBox is on CAN0.
- Consume PedalBox frames from the `evsn_can0_drive_bus_rt` read-only mirror or
  local fanout, not from a generic CAN0 gateway TX/RX path.
- Filter only approved pedal frame IDs. The legacy scalar driver consumed TPDO
  `0x185`; `VSN-0035` owns the CANopen stack decision and any optional
  management COB-IDs.
- Rewrite calibration and disconnect detection as pure logic.
- Publish typed `EVSNPedalState` or a revised RTI-approved IDL type.
- Include accelerator validity, brake status validity, timestamps, input age,
  source CAN ID, diagnostics, and sample counter.
- Unit-test calibration bounds, channel disagreement, stale input, malformed
  payload, and disconnect timer behavior.

## Steerwheel Driver Plan

- Preserve the old launch assignment: steerwheel is on CAN3, even if older
  diagrams or comments imply otherwise.
- Subscribe to raw CAN from the CAN3 gateway and filter legacy frame IDs
  `0x183` for regen and `0x283` for buttons/mode.
- Decode indicator, headlight, horn, interior lights, cruise buttons, gear,
  drive mode, and regen input into a typed input state.
- Replace old scalar topics with a compact typed DDS state and optional derived
  command topics only after contract review.
- Horn output is decoded only as an input fact if needed for diagnostics; it
  must not reintroduce the retired horn command path.
- Unit-test bitfield extraction, regen calibration, disconnected state, stale
  input, and ignored frame IDs.

## Subagents and Skills

- `researcher`: inspect one driver at a time and produce field/topic/CAN ID
  mapping.
- `implementer`: one driver package per assignment.
- `reviewer`: verify stale behavior, calibration bounds, DDS evidence, and
  tests.
- Required skills: `rti-connext-dds`, `middleware-testing`,
  `cpp-unit-testing`, `cpp-integration-testing`, `test-coverage-evidence`,
  `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on pedal or steerwheel unit tests alone.

Minimum evidence:

- smoke tests for each input driver with fake CAN inputs and deterministic
  readiness/exit behavior
- unit tests for calibration, disconnect, bitfields, gear/mode/buttons, invalid
  DLC, malformed payloads, stale inputs, and diagnostics
- hardware-equivalence regression for PedalBox `0x185` on CAN0 and steerwheel
  `0x183`/`0x283` on CAN3, including launch bus assignment checks
- DDS integration tests with real RTI entities after the RTI gate clears
- regression tests proving remote-driving paths are absent and input snapshots
  cannot own final CAN0 motor command TX
- exact commands, test vectors, middleware domain/transport, logs, and gaps
  recorded before status changes

## Steps

- [ ] Complete topic/type registry entries under `VSN-0021`.
- [ ] Port pedal pure logic and tests.
- [ ] Port steerwheel pure logic and tests.
- [ ] Add hardware-equivalence checks for CAN0 PedalBox and CAN3 steerwheel
      bus assignments from `system_launch.xml`.
- [ ] Add DDS adapters after RTI MCP guidance is recorded.
- [ ] Add isolated-domain integration tests for each driver.
- [ ] Document dependency from motor-control snapshot ingestion to pedal state.

## Verification

- [ ] Smoke: each driver validates config and exits boundedly.
- [ ] Unit: pedal and steerwheel decode/control tests.
- [ ] Regression: old PedalBox/steerwheel frame vectors and bus assignments.
- [ ] Integration: raw CAN DDS input to typed input DDS output.
- [ ] QNX target: startup/shutdown and stale-state smoke when available.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0022`, `VSN-0023`, and RTI MCP guidance.
