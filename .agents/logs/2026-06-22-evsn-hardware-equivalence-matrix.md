# 2026-06-22 - EVSN Hardware-Equivalence Matrix

Work items: `VSN-0020`, `VSN-0033`; related `VSN-0019`,
`VSN-0021` through `VSN-0032`, `VSN-0035`, `VSN-0036`, and shelved
`VSN-0037`.

## Request

Update the EVSN network migration execution plans so unchanged hardware
equivalence is explicit. The plans must include a matrix copied from
EMBD-v2-HighLevel launch/config: per bus, per frame ID, direction, old ROS2
topic names, new QNX process owner, DDS topic if any, and a regression test.

## Sources Inspected

- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `../Sunswift/EMBD-v2-HighLevel/src/can_broker/`
- `../Sunswift/EMBD-v2-HighLevel/src/motor_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/pedal_box_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/steerwheel_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/orion2_bms_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/brake_pressure_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/solar_current_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/irradiance/`
- `../Sunswift/EMBD-v2-HighLevel/src/mppt_drivers/`
- `../Sunswift/EMBD-v2-HighLevel/src/indicator_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/taillight_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/headlight_driver/`
- Existing EVSN execution plans under `.agents/exec_plans/`

## External Documentation Evidence

QNX online documentation consulted for the planning context:

- QNX Product Documentation index:
  `https://www.qnx.com/developers/docs/index.html`
- QNX SDP 8.0 documentation:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
- QNX Neutrino IPC:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/ipc.html`
- QNX Neutrino resource managers:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/resource.html`
- QNX scheduling policies:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/overview_SCHEDS.html`
- QNX CAN devctl documentation:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can.html`

RTI Connext MCP status:

- Queried tool discovery for RTI Connext MCP guidance; no RTI MCP tool was
  exposed in this session.
- DDS topic names in the hardware-equivalence matrix remain provisional until
  the RTI Connext MCP guard is satisfied.
- This update changes planning artifacts only and does not create or modify DDS
  generated code, QoS profiles, participants, readers, writers, or runtime DDS
  entities.

## Changes

- Added the hardware-equivalence matrix to
  `.agents/exec_plans/evsn-network-port-roadmap.md`.
- Updated CAN0 plans so `evsn_can0_drive_bus_rt` owns the drive-control-bus HAL
  endpoint, final CAN0 writes, and receive fanout/mirror behavior.
- Preserved WS22 compatibility details: command `0x501`, optional power/reset
  `0x502`/`0x503`, left telemetry `0x400..0x417`, and right telemetry
  `0x420..0x437`.
- Corrected Orion2 BMS frame range to `0x620..0x629`.
- Retired the old steering ECU/EPAS path after user confirmation that the
  steering actuator motor has been removed. CAN1 steering frames `0x290`,
  `0x292`, and `0x298` plus `/control/steering/angle` are retained only as
  retired-reference evidence.
- Removed the old USB/serial `display_driver` output path from MVP scope and
  updated VSN-0026 to cover active lighting drivers only. The future driver
  display is shelved to `VSN-0037` as an HDMI Qt/QML frontend.
- Ran reviewer subagent `019eed7c-4c11-7592-8cd7-41ed5a9ce327`; it returned
  `planning-equivalent` for the active EMBD-v2-HighLevel network minus retired
  systems, with no active non-retired old surface missing from the EVSN plans.
- Added a launch-file traceability checklist to the roadmap and a matching
  regression requirement in VSN-0033.
- Propagated matrix references into CAN broker, CAN HAL, IDL, motor,
  PedalBox/CANopen, GPS, sensors/MPPT, lighting, drive, cruise,
  noncritical, and regression plans.
- Added `REG-HW-*` expectations to the network regression suite so launch/config
  bus assignment, frame IDs, direction, old ROS2 source topics, QNX owner,
  candidate DDS topic, and test ID are all auditable.
- Updated `.agents/agent-context.md`, `.agents/progress.md`,
  `.agents/session-handoff.md`, and `.agents/feature_list.json` with the
  matrix decision and evidence.

## Open Decisions

- Proprietary Audi headlight documentation and generated frame code remain
  local-only and must not be included in public/cloud artifacts.
- Final DDS type names, topic names, QoS, deadline/liveliness, and generated
  integration remain blocked on RTI Connext MCP guidance.
- The reviewer verdict is planning equivalence only. Implemented equivalence
  still requires production code, DDS contract finalization, QNX/SIL or target
  timing evidence, protocol evidence, HIL/bench evidence where applicable, and
  VSN-0033 regression automation.

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 36 work items and links in 126 Markdown
  files.
- Re-ran the same checks after steering ECU retirement, display deferral,
  launch-traceability, and subagent-review updates; all passed.
