# VSN-0022 - EVSN CAN Broker and Gateway Adaptation

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Replace the EMBD-v2-HighLevel `can_broker` and `ros2_socketcan` model with the
EVSN CAN HAL and gateway architecture already started by `VSN-0017` and
`VSN-0019`.

This plan adapts the broker to the current EVSN design:

- one generic gateway process or instance owns one non-CAN0 bus
- raw CAN DDS topics remain bus-first: `/can<bus>/from_can_bus` and
  `/can<bus>/to_can_bus`
- DBC/packet pack and unpack belongs to the CAN-ID-owning node
- the hardware-equivalence matrix in
  `.agents/exec_plans/evsn-network-port-roadmap.md` is the bus/frame source of
  truth for unchanged hardware
- legacy GPS/GNSS CAN payload decode moves to `VSN-0036`; the broker only
  routes raw frames and may apply configured filters for `0x700..0x704`
- CAN0 is the drive-control bus. CAN0 TX is owned by
  `evsn_can0_drive_bus_rt`, not the generic gateway.
- CAN0 motor commands are never routed through this broker or gateway.
- CAN0 raw traffic is distributed to local subscribers by
  `evsn_can0_drive_bus_rt` and may be mirrored to DDS only through a read-only,
  non-command path owned by the CAN0 drive-control bus owner or a strictly
  filtered bridge thread after RTI guidance.

## Non-Goals

- Do not port old `ECU_driver`, IBSE, Elithion/Lithiumate, or motor command
  broker logic.
- Do not decode CAN payloads in the generic gateway.
- Do not reintroduce GPS scalar ROS2 topics from the old broker.
- Do not create a generic TX path for CAN0 motor commands.
- Do not make the generic gateway the CAN0 drive-control bus writer.
- Do not implement source changes until RTI MCP gateway guidance is recorded.

## Context Loaded

- `.agents/exec_plans/evsn-can-gateway.md`
- `.agents/exec_plans/evsn-can-abstraction-layer.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/can_broker/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/ros2_socketcan/`
- `.agents/exec_plans/evsn-gps-can-driver-port.md`
- `../Sunswift/EMBD-v2-HighLevel/docs/physical_architecture.png`

## External Guidance

- RTI MCP: unavailable in this session. Final DDS gateway QoS, resource limits,
  WaitSet/listener behavior, and generated type integration remain blocked.
- QNX online docs: use the QNX CAN `devctl()` docs already recorded in
  `VSN-0017` and `VSN-0019`, plus scheduling and runmask docs listed in
  `VSN-0020`.

## Proposed Architecture

- Keep `VSN-0019` as the low-level CAN HAL owner.
- Keep `VSN-0017` as the generic raw CAN DDS gateway owner for non-CAN0 buses,
  but update it to explicitly exclude CAN0 drive-control TX.
- Add `evsn_can0_drive_bus_rt` under `VSN-0023` as the separate real-time CAN0
  drive-control bus writer and receive-fanout owner. This process is not the
  generic DDS CAN gateway.
- Add a bus-priority configuration:
  - CAN0 drive-control bus writer: owned by `evsn_can0_drive_bus_rt` under
    `VSN-0023`; no DDS command routing.
  - CAN0 telemetry/raw mirror/read-only bridge: high priority, no command TX.
    Local subscriber fanout must cover the CAN0 rows in the roadmap matrix,
    including PedalBox `0x185`, BMS `0x620..0x629`, brake pressure `0x1B3`,
    solar current `0x1B4`, GNSS `0x700..0x704`, WS22 motor telemetry
    `0x400..0x417` and `0x420..0x437`, and optional tyre pressure
    `0x4D0..0x4D3`.
  - CAN1/CAN2/CAN3 body, sensors, and lighting gateways: lower priority than
    CAN0 drive-control and CAN0 telemetry.
- Use bounded queues between DDS callbacks and CAN HAL calls.
- Expose diagnostics for queue pressure, rejected CAN0 TX attempts, malformed
  raw frames, backend errors, stale bus state, and shutdown.

Non-CAN0 gateway routes required for unchanged-hardware MVP:

- CAN1 raw RX/TX for indicators `0x18D`/`0x18E`, taillight `0x18B`,
  irradiance `0x1B7`/`0x1B8`/`0x1B9`/`0x1C0`, TPEE MPPT
  `0x210`/`0x211`/`0x220`/`0x221`, and Elmar MPPT
  `0x600`/`0x601`/`0x602`/`0x605`.
- Retired steering ECU/EPAS frames `0x290`, `0x292`, and `0x298` must have no
  active route-table entry and no generic gateway TX path.
- CAN2 raw TX for the proprietary Audi headlight frame set.
- CAN3 raw RX for steerwheel `0x183` and `0x283`.

## GPS/GNSS CAN Driver Integration

The old `can_broker/gps_driver` is ported under `VSN-0036`, not inside the
generic gateway.

Gateway responsibilities:

- route raw frames from the configured GPS/GNSS bus,
- optionally filter standard IDs `0x700..0x704`,
- publish those frames on the bus-first raw RX topic,
- expose dropped-frame and queue-pressure diagnostics.

GPS/GNSS driver responsibilities:

- own payload decode for IDs `0x700..0x704`,
- validate standard frame format and DLC,
- publish typed navigation/GNSS state after RTI guidance,
- provide package-level smoke, unit, integration, and regression tests.

## Subagents and Skills

- `researcher`: inspect old broker/socketcan topology and identify only active
  bus flows.
- `implementer`: edit only gateway, HAL integration, config, and tests.
- `reviewer`: verify CAN0 command exclusion, QNX docs, RTI MCP evidence,
  bounded queue behavior, and tests.
- Required skills: `rti-connext-dds`, `qnx-cpp-rtos`,
  `qnx-cpp-performance`, `middleware-testing`, `cpp-integration-testing`,
  `test-coverage-evidence`, `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on fake-HAL or unit tests alone.

Minimum evidence:

- smoke tests for gateway startup, configuration validation, readiness, and
  bounded shutdown
- unit tests for bus mapping, queue pressure, malformed frames, backend error
  statuses, and rejected CAN0 motor command TX
- hardware-equivalence routing tests for every CAN0/CAN1/CAN2/CAN3 row in the
  roadmap matrix, including launch/config bus assignment checks
- DDS integration tests with real RTI entities for non-motor bus RX/TX after
  the RTI gate clears
- QNX/SIL `dev-can-linux` smoke with loopback frames, endpoint names, status
  codes, and timing evidence when the target contract exists
- regression tests proving generic gateway, cloud, telemetry, and body-control
  paths cannot send final CAN0 motor commands

## Steps

- [ ] Update `VSN-0017` gateway plan to reference this CAN0 exclusion model.
- [ ] Add config validation that refuses generic CAN0 TX routes unless
      explicitly in read-only mirror mode.
- [ ] Add raw CAN topic registry entries for each non-motor bus.
- [ ] Add a gateway route table generated from, or checked against, the
      roadmap hardware-equivalence matrix.
- [ ] Add GPS/GNSS raw RX routing config for `VSN-0036` without broker-side
      payload decode.
- [ ] Run RTI MCP and record gateway QoS/lifecycle guidance.
- [ ] Implement or update gateway package after RTI gate clears.
- [ ] Add fake-HAL unit tests for queue pressure and command rejection.
- [ ] Add DDS integration tests for non-motor bus RX/TX.
- [ ] Add QNX/SIL smoke with `dev-can-linux -L` and evidence capture.

## Verification

- [ ] Command: `./init.sh check`
      Result:
- [ ] Command: `git diff --check`
      Result:
- [ ] Command: focused CAN gateway unit tests
      Result:
- [ ] Command: hardware-equivalence gateway routing regression
      Result:
- [ ] Command: isolated-domain DDS gateway integration test
      Result:
- [ ] Command: QNX/SIL `dev-can-linux` smoke
      Result:

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

This supersedes any old assumption that `can_broker` owns motor commands.
Implementation remains blocked on RTI MCP guidance.
