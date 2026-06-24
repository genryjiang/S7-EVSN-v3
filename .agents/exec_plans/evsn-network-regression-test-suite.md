# VSN-0033 - Network Regression and Integration Test Suite

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: in_progress

## Scope

Create a separate strict verification work item for the EVSN network migration.
This plan owns cross-package integration, regression, evidence capture, and
test-gate expectations for `VSN-0019` and `VSN-0021` through `VSN-0032`.

The suite must prove that migrated drivers, controllers, middleware contracts,
CAN paths, telemetry adapters, and cloud/DB bridges work together without
reintroducing ROS2 behavior, branding, or unsafe CAN0 motor-command routing.

## Non-Goals

- Do not use this work item to implement production driver/controller logic.
- Do not treat test evidence alone as MISRA C++ compliance or ISO 26262
  compliance.
- Do not run HIL, live vehicle, or destructive target tests without a separate
  approved target contract.
- Do not relax the RTI Connext MCP or QNX online-documentation gates for
  implementation work items.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-network-contracts-idl.md`
- `.agents/exec_plans/evsn-can-broker-adaptation.md`
- `.agents/exec_plans/evsn-motor-control-critical-path.md`
- `.agents/exec_plans/evsn-orion2-bms-driver-port.md`
- `.agents/exec_plans/evsn-input-drivers-port.md`
- `.agents/exec_plans/evsn-display-lighting-drivers-port.md`
- `.agents/exec_plans/evsn-sensor-mppt-drivers-port.md`
- `.agents/exec_plans/evsn-drive-controller-port.md`
- `.agents/exec_plans/evsn-cruise-controller-port.md`
- `.agents/exec_plans/evsn-noncritical-controller-port.md`
- `.agents/exec_plans/evsn-energy-telemetry-ittia-port.md`
- `.agents/exec_plans/evsn-cloud-telemetry-bridge-port.md`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix

Skills and references loaded:

- `harness-creator`
- `test-coverage-evidence`
- `cpp-integration-testing`
- `middleware-testing`
- Coverage criteria, test automation, simulation/SIL/HIL, DDS middleware, and
  MQTT middleware testing references from those skills.

## Test Policy

Each migrated work item must provide traceable evidence at these levels:

- Smoke: process/package starts with fake inputs, validates configuration,
  reports readiness, and exits cleanly within a deterministic timeout.
- Unit: pure decode, encode, validation, state-machine, limit, stale-data, and
  fault-path logic is tested without DDS, QNX, CAN hardware, cloud, or database
  dependencies.
- Middleware integration: real RTI Connext participants/readers/writers run in
  isolated domains with explicit topic, IDL, QoS, deadline, liveliness, and
  stale-data expectations after the RTI gate clears.
- Adapter integration: fake CAN, fake DB, fake cloud, and QNX/SIL adapters are
  tested with deterministic injected inputs and bounded failures.
- Regression: package-level tests are composed into network scenarios that
  verify boot, startup order, shutdown, stale data, malformed inputs, reconnect,
  rejected commands, and safe degraded behavior.
- Evidence: every run records command, platform, target/configuration,
  middleware transport/domain, inputs, outputs, logs/traces, exit status,
  residual risks, and known gaps.

Tests must avoid shared mutable state, unbounded sleeps, nondeterministic
timeouts, hidden external services, and order dependence. Golden reference
updates require explicit review evidence.

## Regression Matrix

- Hardware-equivalence regression:
  - Assert the launch-file traceability checklist in `VSN-0020`: every active,
    commented, or user-retired launch surface in `system_launch.xml` has exactly
    one EVSN owner or explicit retirement/optional decision.
  - Parse or otherwise assert the unchanged-hardware matrix from `VSN-0020`:
    every row has bus/surface, frame ID or non-CAN surface, direction, old
    ROS2 topic/source, new QNX owner/process, candidate DDS topic or "none",
    and regression ID.
  - Run golden old-to-new tests for active CAN frames:
    CAN0 `0x501`, optional `0x502`/`0x503`, `0x400..0x417`,
    `0x420..0x437`, `0x185`, `0x620..0x629`, `0x1B3`, `0x1B4`,
    `0x700..0x704`; CAN1 `0x18D`, `0x18E`, `0x18B`,
    `0x1B7`, `0x1B8`, `0x1B9`, `0x1C0`, `0x210`, `0x211`,
    `0x220`, `0x221`, `0x600`, `0x601`, `0x602`, `0x605`;
    CAN2 Audi headlight frame set; CAN3 `0x183`, `0x283`.
  - Verify launch/config bus assignments: PedalBox/BMS/brake/solar/GNSS/motor
    telemetry on CAN0, indicators/taillight/irradiance/MPPTs on CAN1,
    headlights on CAN2, steerwheel on CAN3.
  - Verify optional or excluded surfaces are explicit: tyre pressure is disabled
    for the launch-file MVP unless current config enables it; steering EPAS/ECU
    frames `0x290`, `0x292`, and `0x298` are retired because the actuator motor
    has been removed and must have no active route, DDS command topic, or
    compatibility shim.
  - Verify old ROS2 topic names are accepted only as test-vector labels or
    migration evidence and do not appear as public EVSN runtime contracts.
- Contract regression:
  - IDL generation/build for all active custom types.
  - Topic registry scan for removed branded names and retired packages.
  - Negative checks for retired ROS2 topic names and prohibited CAN0 TX topics.
  - DDS compatibility checks for matching and intentionally mismatched QoS or
    type/schema where practical.
- CAN regression:
  - Fake HAL RX/TX for standard, extended, RTR, DLC, malformed, and backend
    error cases.
  - Generic gateway refuses CAN0 motor command TX.
  - CAN0 drive-control bus owner fanout delivers CAN0 RX frames to subscribers
    without exposing a subscriber TX endpoint.
  - CAN0 telemetry mirror, if present, is read-only for motor-command IDs.
  - QNX/SIL `dev-can-linux` smoke captures endpoint names, loopback frames,
    status codes, and timing evidence.
- Driver regression:
  - BMS, pedal, steerwheel, GPS/GNSS, lighting, sensor, MPPT, and headlight
    packages each run package smoke, pure unit, and DDS/fake-CAN integration
    tests.
  - Proprietary Audi headlight files are included only in local/protected test
    paths and excluded from public/cloud artifact scans.
  - Centre-display and old USB/serial `display_driver` output paths must not
    appear in MVP runtime contracts, route tables, or active package manifests.
    Future HDMI Qt display work is deferred to `VSN-0037` and has its own
    activation tests.
- Controller regression:
  - Drive, cruise, and non-critical body controllers cover stale input,
    contradictory input, mode transitions, limits, disables, and safe outputs.
  - No controller test may assert ownership of final CAN0 motor command TX.
- Motor critical-path regression:
  - Pure control logic covers safe state, stale snapshots, transmit failure,
    contradictory inputs, deadline miss accounting, and bounded outputs.
  - WS22 `0x501` golden frames match the old velocity/current float layout and
    current clamp/scale behavior for approved vectors.
  - Negative tests reject old cloud-originated motor reset, motor power, motor
    limit, torque/current/velocity, drive-enable, and final CAN0 command
    requests unless a separate approved command-authority plan exists.
  - QNX timing smoke records period, compute time, transmit time, jitter, and
    deadline misses with `tracelogger` when a target contract exists.
  - DDS status publication is verified as a copied, lower-priority path that
    cannot back-pressure the hot loop.
- Telemetry regression:
  - ITTIA DB adapter tests cover schema open/create, write, readback, reset,
    close, full/locked/error cases, and fake DB replay.
  - Cloud bridge tests cover allowlists, batching, reconnect/offline buffering,
    duplicate delivery handling, LWT/status behavior where applicable, and no
    final motor-command path.
  - Cloud bridge tests explicitly reject old cloud-originated motor reset,
    motor power, motor limit, torque/current/velocity, drive-enable, and final
    CAN0 command paths unless a separate approved command-authority plan exists.

## Subagents and Skills

- `test-architect`: owns the regression matrix, labels, fixtures, and evidence
  template. Does not edit production logic.
- `middleware-tester`: owns DDS/MQTT contract and negative tests.
- `integration-tester`: owns host, SIL, QNX target, fake adapter, and startup/
  shutdown scenarios.
- `reviewer`: verifies traceability, platform evidence, missing-test gaps,
  flaky-test risk, CAN0 exclusion, and proprietary-artifact scans.
- Required skills: `test-coverage-evidence`, `cpp-integration-testing`,
  `middleware-testing`, `safety-critical-cpp`.

## Steps

- [ ] Define test labels and commands for unit, smoke, middleware integration,
      SIL/QNX, HIL/manual, and full network regression tiers.
- [ ] Add a regression evidence template with platform, target, transport,
      domain, inputs, outputs, logs/traces, exit status, and residual risks.
- [ ] Add a package checklist that every VSN-0021 through VSN-0032 work item
      must satisfy before moving out of implementation.
- [ ] Add a hardware-equivalence fixture/table and golden-vector loader keyed
      by the `REG-HW-*` IDs in `VSN-0020`.
- [ ] Build fake-adapter fixtures for CAN, DB, and cloud paths.
- [ ] Add DDS isolated-domain fixtures after RTI Connext guidance is recorded.
- [ ] Add negative tests for banned branding, retired packages, prohibited
      CAN0 motor TX topics, malformed frames, stale data, and QoS/schema
      mismatches where practical.
- [ ] Add QNX/SIL and target timing evidence hooks for CAN gateway and motor
      critical-path work.
- [ ] Wire the fast regression tier into the standard local verification gate,
      and keep slower QNX/HIL tiers opt-in with explicit evidence requirements.

## Verification

- [ ] Command: `./init.sh check`
      Result:
- [ ] Command: `git diff --check`
      Result:
- [ ] Command: fast network regression suite
      Result:
- [ ] Command: DDS isolated-domain integration suite
      Result:
- [ ] Command: fake CAN/DB/cloud adapter integration suite
      Result:
- [ ] Command: hardware-equivalence regression suite
      Result:
- [ ] Command: QNX/SIL gateway and motor timing smoke when target is available
      Result:

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-regression-tests.md`

## Handoff

Implementation work items may proceed independently, but none of the network
port work should be called complete until its package-level evidence is linked
to this regression suite or a concrete, documented blocker is recorded.
