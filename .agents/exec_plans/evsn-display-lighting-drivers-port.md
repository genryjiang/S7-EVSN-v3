# VSN-0026 - Headlight, Indicator, and Taillight Driver Ports

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port active body lighting drivers:

- `headlight_driver`
- `indicators_driver`
- `taillight_driver`

New packages must use generic EVSN naming and typed DDS contracts. The
headlight package contains proprietary Audi headlight material and must remain
local-only. The old USB/serial `display_driver` output path is not part of the
MVP port. The future driver display is shelved to `VSN-0037` as an HDMI Qt/QML
frontend and must be implemented last.

## Non-Goals

- Do not port `horn_driver`, `centre_display`, or the old USB/serial
  `display_driver` output path.
- Do not add Qt, HDMI, or display-snapshot code in this work item; that belongs
  to deferred `VSN-0037`.
- Do not upload proprietary Audi headlight docs, generated CAN code, or usage
  notes to any public cloud instance or public artifact.
- Do not preserve old branded text in new public package names, topics, cloud
  paths, or docs.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/headlight_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/indicators_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/taillight_driver/`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_logic/non_critical_controller/`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix

## External Guidance

- RTI MCP: unavailable. DDS type/topic/QoS implementation is blocked.
- QNX online docs: use roadmap QNX docs for runtime and scheduling behavior.

## Proprietary Headlight Driver Plan

- Port local docs and usage from `README.md` and `audi_headlights_readme.txt`
  into a private local docs path in this repo.
- Isolate Audi generated CAN database, CRC, counter, facade, and message
  helpers in a package that is excluded from public cloud sync/deployment.
- Preserve the old launch assignment: headlights are on CAN2. The old driver
  default of CAN1 must not be used for unchanged hardware.
- Subscribe to generic lighting command DDS state and publish raw CAN frames to
  the configured non-motor bus.
- Preserve the old Audi command frame set listed in the roadmap matrix,
  including standard IDs such as `0x40`, `0xFD`, `0x15F`, `0x190`,
  `0x192`, `0x193`, `0x195`, `0x3C0`, `0x585`, `0x6B2`, MXB IDs
  `0x160..0x169`, `0x16A..0x16F`, `0x174`, `0x175`, and the documented
  extended IDs.
- Unit-test CRC, counters, pack/unpack, command mapping, malformed command
  rejection, and repeated frame timing.

## Indicator Driver Plan

- Replace `/control/indicator/*` scalar subscriptions with typed lighting
  command input.
- Preserve the old launch assignment: side indicators are on CAN1.
- Publish raw CAN frames for left ID `0x18D` and right ID `0x18E` through the
  CAN1 raw CAN gateway.
- Unit-test on/off mapping, ignored invalid states, and bus config validation.

## Taillight Driver Plan

- Replace scalar subscriptions for tail, brake, reverse, and indicators with a
  typed lighting command state.
- Preserve the old launch assignment: taillight is on CAN1, even though the
  old driver default was CAN0.
- Publish raw CAN ID `0x18B` using fixed payload mapping.
- Unit-test all bit combinations, repeated publish policy, malformed command
  rejection, and startup safe output.

## Subagents and Skills

- `researcher`: inspect one driver package and produce field/CAN/topic mapping.
- `implementer`: one driver package per assignment.
- `reviewer`: verify proprietary isolation, branding removal, CAN mapping,
  QNX docs, RTI MCP evidence, and tests.
- Required skills: `rti-connext-dds`, `qnx-cpp-rtos`, `middleware-testing`,
  `cpp-unit-testing`, `cpp-integration-testing`, `test-coverage-evidence`,
  `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on lighting unit tests alone.

Minimum evidence:

- smoke tests for headlight, indicator, and taillight packages with
  fake inputs and bounded startup/shutdown
- unit tests for command mapping, bit packing, DLC checks, malformed payloads,
  stale inputs, diagnostics, and proprietary headlight helper behavior
- hardware-equivalence regression for CAN2 headlight frame sequences, CAN1
  indicator `0x18D`/`0x18E`, CAN1 taillight `0x18B`, and launch/config bus
  assignments
- DDS-to-fake-CAN integration tests with real RTI entities after the RTI gate
  clears
- regression scans proving proprietary Audi headlight artifacts are local-only
  and absent from public/cloud artifacts
- regression tests proving horn, centre-display, and old USB/serial
  `display_driver` output paths are not reintroduced, while the HDMI Qt display
  remains explicitly deferred to `VSN-0037`

## Steps

- [ ] Define typed lighting IDL under `VSN-0021`.
- [ ] Add local-only proprietary headlight artifact policy.
- [ ] Add hardware-equivalence vectors for CAN2 headlights and CAN1
      indicator/taillight frames.
- [ ] Port pure command-to-frame logic for headlights, indicators, and
      taillights.
- [ ] Add DDS adapters after RTI MCP guidance is recorded.
- [ ] Add smoke, unit, and integration tests for each active lighting driver.

## Verification

- [ ] Smoke: each active lighting driver validates config and exits boundedly.
- [ ] Unit: Audi helpers, indicator frames, taillight frames.
- [ ] Regression: hardware-equivalence lighting frame and bus matrix.
- [ ] Integration: typed DDS commands to raw CAN DDS frames using fake gateway.
- [ ] Policy: scan public cloud/deployment artifacts for Audi/proprietary files.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0022`, `VSN-0028`, and RTI MCP guidance.
