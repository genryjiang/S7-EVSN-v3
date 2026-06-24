# VSN-0027 - Sensor and MPPT Driver Ports

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port active sensor and MPPT drivers:

- `brake_pressure_sensor_driver`
- `solar_current_driver`
- `irradiance_driver`
- `tyre_pressure_driver`
- `tpee_mppt_driver`
- `elmar_mppt_driver`

Each driver must own its CAN-ID filtering and decode logic locally and publish
typed EVSN sensor/energy DDS outputs.

## Non-Goals

- Do not port `suspension_driver`.
- Do not preserve old `/car/...` or `/sensors/...` names as public contracts
  without generic EVSN topic review.
- Do not route sensor traffic through CAN0 motor command TX.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/sensors/brake_pressure_sensor_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/sensors/solar_current_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/sensors/irradiance_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/sensors/tyre_pressure_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/sensors/tpee_mppt_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/sensors/elmar_mppt_driver/`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix

## External Guidance

- RTI MCP: unavailable. DDS type/topic/QoS implementation is blocked.
- QNX online docs: use roadmap QNX docs for runtime behavior and CAN gateway
  docs from `VSN-0017`/`VSN-0019`.

## Per-Driver Plans

Brake pressure:

- Preserve the old launch assignment: brake pressure is on CAN0.
- Decode configured brake pressure CAN frame, historically node ID `0x1B3`,
  from the `evsn_can0_drive_bus_rt` read-only mirror/fanout.
- Publish typed brake pressure with units, validity, timestamp, and stale flag.
- Test malformed payload, wrong ID, disconnected timeout, and threshold users.

Solar current:

- Preserve the old launch assignment: solar current is on CAN0.
- Decode solar-current board frame `0x1B4` from the CAN0 owner mirror/fanout
  and publish typed current.
- The old driver created `/can0/to_can_bus` for a TODO calibration function.
  Do not preserve CAN0 TX unless device documentation or bench evidence proves
  a real calibration/request protocol.

Irradiance:

- Preserve the old launch assignment: irradiance is on CAN1.
- Decode middle-right `0x1B7`, back-right `0x1B8`, front-right `0x1B9`, and
  middle-left `0x1C0` irradiance CAN frames.
- Publish typed raw and GSI values with sensor position enum.

Tyre pressure:

- Decode `0x4D0..0x4D3` for LF/RF/LR/RR tyre data.
- Replace color/scalar ROS2 topics with a typed tyre sensor state.
- The old `system_launch.xml` comments out the tyre pressure launch. Treat this
  as disabled for the MVP unless the current hardware/config explicitly enables
  it, but keep the decode plan ready because the package exists.

TPEE MPPT:

- Preserve the old launch assignment: TPEE MPPT is on CAN1.
- Decode IDs `0x210`, `0x211`, `0x220`, and `0x221`.
- Publish typed MPPT input/output voltage/current, mode, fault, enable, and
  temperature states for MPPT1 and MPPT2.

Elmar MPPT:

- Preserve the old launch assignment: Elmar MPPT is on CAN1.
- Decode IDs `0x600`, `0x601`, `0x602`, and `0x605`.
- Publish typed MPPT3 voltage/current, temperature, status, and fault state.

## Subagents and Skills

- `researcher`: one assigned driver only, produce CAN ID and field mapping.
- `implementer`: one driver package per assignment.
- `reviewer`: verify decode coverage, stale behavior, RTI/QNX evidence, and
  tests.
- Required skills: `rti-connext-dds`, `middleware-testing`,
  `cpp-unit-testing`, `cpp-integration-testing`, `test-coverage-evidence`,
  `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on shared helper or happy-path decode tests alone.

Minimum evidence:

- smoke tests for every included sensor and MPPT driver with fake CAN inputs
  and bounded startup/shutdown
- unit tests for each driver's CAN IDs, DLC, scaling, units, ranges,
  freshness, diagnostics, malformed frames, and boundary values
- hardware-equivalence regression for the launch/config bus assignment and
  frame set for brake pressure `0x1B3`, solar current `0x1B4`, irradiance
  `0x1B7`/`0x1B8`/`0x1B9`/`0x1C0`, TPEE MPPT
  `0x210`/`0x211`/`0x220`/`0x221`, Elmar MPPT
  `0x600`/`0x601`/`0x602`/`0x605`, and optional tyre pressure
  `0x4D0..0x4D3`
- DDS integration tests with real RTI entities after the RTI gate clears
- regression tests proving suspension, retired sensor systems, and branded
  topic names are absent
- exact commands, per-driver test vectors, middleware domain/transport, logs,
  and known gaps recorded before status changes

## Steps

- [ ] Define generic sensor and MPPT IDL under `VSN-0021`.
- [ ] Port each pure decode module with fixed payload validation.
- [ ] Add bus/frame compatibility tests from the roadmap hardware-equivalence
      matrix.
- [ ] Add DDS adapters after RTI MCP guidance is recorded.
- [ ] Add fake raw CAN integration tests per driver.
- [ ] Add README evidence per driver with CAN IDs, units, and stale behavior.

## Verification

- [ ] Smoke: each driver validates config and exits boundedly.
- [ ] Unit: decode, ignored IDs, malformed DLC, boundary values, stale state.
- [ ] Regression: hardware-equivalence sensor/MPPT frame matrix.
- [ ] Integration: raw CAN DDS input to typed sensor/MPPT DDS output.
- [ ] QNX target: startup/shutdown smoke when target is available.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0022`, and RTI MCP guidance.
