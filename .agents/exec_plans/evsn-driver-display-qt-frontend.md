# VSN-0037 - Deferred HDMI Qt Driver Display Frontend

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: shelved

## Scope

Shelve the old EMBD-v2-HighLevel `display_driver` port until after the
network/control MVP is implemented and regression-tested. When resumed, rewrite
driver display as a new HDMI-connected Qt/QML frontend on QNX instead of
porting the old USB/serial display path.

This is intentionally last-priority UI work. It must not block CAN, motor,
PedalBox, BMS, GNSS, lighting, telemetry, or regression-suite MVP work.

## Non-Goals

- Do not port the old USB/serial `display_driver` output path.
- Do not port `centre_display`.
- Do not add display code to `VSN-0026`; that work item remains lighting only.
- Do not let UI consume final motor-command paths or publish vehicle-control
  commands.
- Do not let Qt, HDMI, graphics, logging, or UI rendering run inside real-time
  control paths.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-network-contracts-idl.md`
- `.agents/exec_plans/evsn-network-regression-test-suite.md`
- `.agents/exec_plans/evsn-display-lighting-drivers-port.md`
- `.agents/agent-context.md`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/display_driver/`

Skills and references loaded:

- `harness-creator`
- `qt-qml`
- `qt-ui-design`

## External Guidance

- QNX Product Documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- QNX SDP 8.0 Graphics documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/ui.html
- QNX Screen Graphics Subsystem Developer's Guide:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen/topic/manual/cscreen_about.html
- QNX OpenWF Display Server User's Guide:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen.wfd-server/topic/manual/cwfd-server_about.html
- QNX OpenWF Display Configuration Developer's Guide:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen.wfdcfg/topic/manual/cwfdcfg_about.html
- Qt for QNX:
  https://doc.qt.io/qt-6/qnx.html
- Qt for QNX runtime target requirements:
  https://doc.qt.io/qt-6/qnx-target-requirements.html

Design consequences:

- The target must bring up QNX Screen for HDMI graphics before any Qt
  application can be considered runnable.
- HDMI/display configuration belongs to QNX graphics configuration and target
  image/startup work, not the vehicle real-time control loop.
- The Qt app needs target-runtime prerequisites such as Screen, Qt runtime and
  QML paths, physical screen size/DPI, logging, `/dev/random`, and any required
  networking services.
- The display mode, HDMI timing, resolution, DPI, orientation, brightness, input
  method, GPU availability, and target Qt/QNX versions are blockers until
  provided by hardware evidence.

## Proposed Architecture

- Add a low-priority `evsn_driver_display_qt` process after MVP.
- Use Qt Quick/QML for the UI and render over HDMI through QNX Screen.
- Feed the UI from copied EVSN display snapshots only. The snapshot adapter may
  subscribe to approved DDS telemetry/status topics after RTI guidance clears,
  but it must never back-pressure control, CAN0, or telemetry storage paths.
- Keep the display process outside real-time scheduling groups and off any CPU
  core reserved for motor control or CAN0 drive-control bus ownership.
- Keep all vehicle-state formatting, unit conversion, stale-state indicators,
  and alarm presentation in pure testable logic outside QML where practical.
- Use QML for presentation and lightweight UI state only; avoid expensive
  bindings, unnecessary effects, and dynamic object creation in frequently
  updated views.
- The safe UI default is a degraded/status screen when snapshots are stale,
  missing, or contradictory.

## UI Direction

- Primary information: speed, drive mode/gear, BMS pack state, motor status,
  critical warnings, GNSS validity, and key lighting/body status.
- Secondary information: MPPT/solar state, temperatures, cloud/telemetry link,
  and detailed diagnostics.
- Tertiary information: configuration/build/version data and service-only
  debugging screens.
- Use fixed-layout embedded-screen assumptions until hardware resolution,
  DPI, viewing distance, and input method are confirmed.
- Use persistent status and alarm regions. Safety-relevant state must use
  color plus shape/icon/text, never color alone.
- Avoid hover-only interactions. Touch or hardware-button interaction must be
  explicit if the final display has input.

## Subagents and Skills

- `researcher`: inspect old display topics and identify only the data that
  should become display snapshots.
- `designer`: produce screen inventory, priority, typography, colors,
  input/focus model, stale-state and warning behavior.
- `implementer`: build Qt/QML app, snapshot adapter, tests, and QNX packaging
  only after this work is unshelved.
- `reviewer`: verify UI cannot affect control paths, QNX graphics evidence,
  Qt/QML performance, accessibility, stale-state behavior, and tests.
- Required skills when unshelved: `qt-ui-design`, `qt-qml`,
  `rti-connext-dds`, `qnx-cpp-rtos`, `middleware-testing`,
  `cpp-unit-testing`, `cpp-integration-testing`, `test-coverage-evidence`,
  `safety-critical-cpp`.

## Strict Verification Gate

This work item is not part of the MVP regression pass. Before activation:

- smoke test proving the Qt process starts, reaches a known screen, and exits
  boundedly on host or QNX/SIL
- pure unit tests for display snapshot selection, formatting, units,
  stale-data behavior, warnings, and degraded screen selection
- QML/UI tests for screen routing, focus order, text fit, stale-state
  rendering, and warning presentation
- adapter integration tests with fake DDS/display snapshots after RTI guidance
  clears
- QNX Screen/HDMI target smoke proving the configured display mode, physical
  screen size, Qt runtime paths, and rendering pipeline are correct
- regression scan proving old USB/serial display paths and centre-display
  contracts remain absent
- evidence that the display process cannot publish control commands and cannot
  back-pressure CAN0, motor-control, or telemetry ingestion

## Steps

- [ ] Keep work shelved until network/control MVP and `VSN-0033` regression
      foundations are complete.
- [ ] Confirm hardware: HDMI output path, display panel, resolution, DPI,
      orientation, brightness controls, GPU availability, input method, and
      mounting/viewing distance.
- [ ] Confirm Qt/QNX licensing, target architecture, Qt version, QNX SDP
      version, QNX Screen support, and target image/startup ownership.
- [ ] Define display snapshot IDL/registry entry under `VSN-0021` after RTI
      MCP guidance.
- [ ] Produce a Qt/QML screen inventory and UI design plan.
- [ ] Build fake snapshot and host-render tests before QNX target work.
- [ ] Add QNX Screen/HDMI target smoke and capture evidence.
- [ ] Only then implement production display frontend packaging/startup.

## Verification

- [ ] Command: `./init.sh check`
      Result:
- [ ] Command: `git diff --check`
      Result:
- [ ] Command: Qt/QML lint or QML test suite
      Result:
- [ ] Command: fake display snapshot unit/integration tests
      Result:
- [ ] Command: QNX Screen/HDMI target smoke when hardware exists
      Result:

## Evidence

- Planned log: `.agents/logs/2026-06-22-evsn-driver-display-qt-frontend.md`

## Handoff

This work is deliberately last. Resume only after the core network/control MVP,
RTI contract work, and regression foundations are stable.
