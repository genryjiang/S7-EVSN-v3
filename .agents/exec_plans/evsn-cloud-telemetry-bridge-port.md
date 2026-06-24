# VSN-0032 - Generic Vehicle Cloud Telemetry Bridge

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port the active parts of `iotcore_bridge` as a generic vehicle-cloud telemetry
bridge. The bridge must consume typed EVSN DDS telemetry and publish only
approved generic cloud payloads. Branded thing names, topics, certificates, and
bucket/org names must be removed.

## Non-Goals

- Do not port remote-driving commands.
- Do not port public cloud upload of proprietary Audi headlight docs/code.
- Do not route motor commands from cloud or telemetry into CAN0.
- Do not accept, translate, forward, replay, or synthesize cloud-originated
  motor reset, motor power, motor limit, torque/current/velocity, drive enable,
  or final CAN0 command requests unless a future safety review and execution
  plan explicitly approve a separate command-authority path.
- Do not auto-discover every DDS topic at runtime without an allowlist.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-energy-telemetry-ittia-port.md`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_telemetry/iotcore_bridge/`

## External Guidance

- RTI MCP: unavailable. DDS subscriptions and QoS are blocked until guidance is
  recorded.
- QNX online docs: use roadmap QNX docs for networking/filesystem behavior.

## Proposed Architecture

- Package name: generic, for example `evsn_cloud_telemetry_bridge`.
- Cloud topic prefix and client identity supplied by deployment config, not
  hard-coded.
- DDS topic allowlist generated from `VSN-0021` topic registry.
- Publish telemetry snapshots, system health, and optional odometer status.
- Subscribe only to approved non-safety cloud command topics, if any, and route
  them to parameter/configuration paths, never motor command CAN0.
- Treat any legacy cloud command resembling motor reset, motor power, motor
  limits, torque/current/velocity, drive enable, or CAN0 command as rejected
  input; record diagnostics only.
- Secret/certificate paths are environment or deployment-provided and ignored
  by git.
- Reject or redact proprietary headlight artifacts from cloud payloads.

## Subagents and Skills

- `researcher`: map old cloud topics, allowlist, commands, and branded names.
- `implementer`: port cloud bridge and tests only after cloud contract approval.
- `reviewer`: verify branding removal, secret handling, command restrictions,
  RTI evidence, and tests.
- Required skills: `mqtt-middleware`, `rti-connext-dds`,
  `middleware-testing`, `cpp-integration-testing`,
  `test-coverage-evidence`, `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on serializer or allowlist unit tests alone.

Minimum evidence:

- smoke test for bridge startup with fake DDS inputs and fake cloud/MQTT
  adapter
- unit tests for allowlist generation, serialization, batching, topic mapping,
  reconnect/offline buffering, duplicate delivery handling, retained/LWT
  behavior where applicable, and error propagation
- DDS-to-fake-cloud integration tests with real RTI entities after the RTI gate
  clears
- regression scans for branded topic prefixes, thing names, certificate
  filenames, bucket/org names, and proprietary Audi headlight artifacts
- negative tests proving no cloud command, DDS subscription, or bridge path can
  send final CAN0 motor commands
- negative tests rejecting old cloud-originated motor reset, motor power, motor
  limit, torque/current/velocity, drive enable, and CAN0 command payloads/topics
  unless a future approved command-authority plan exists

## Steps

- [ ] Define generic cloud telemetry schema and topic allowlist.
- [ ] Remove branded `sunswift7`, `Sunswift`, BWSC, and SR names.
- [ ] Define explicit no-proprietary-headlight-upload policy and tests.
- [ ] Add DDS subscribers after RTI MCP guidance is recorded.
- [ ] Add MQTT/cloud adapter with bounded reconnect/offline buffering.
- [ ] Add fake cloud integration tests and credential-path validation.

## Verification

- [ ] Smoke: bridge starts with fake cloud adapter and empty allowlist.
- [ ] Unit: allowlist, payload transformation, branding scan, secret path
      validation, reconnect/drop policy.
- [ ] Integration: DDS input to fake cloud publish with approved payloads only.
- [ ] Policy: scan cloud payload code/tests for proprietary Audi material.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0031`, and RTI MCP guidance. Cloud deployment
details must be supplied outside source control.
