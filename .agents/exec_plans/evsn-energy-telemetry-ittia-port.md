# VSN-0031 - Generic Energy Telemetry with ITTIA DB

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Port `bwsc_energy_telem` as generic EVSN energy telemetry. Replace InfluxDB
with ITTIA DB on QNX and remove BWSC/SR/Sunswift naming from packages, topics,
measurements, and database schema.

## Non-Goals

- Do not use InfluxDB.
- Do not publish or depend on BWSC-specific topic names or reset service names.
- Do not put database writes in real-time control paths.

## Context Loaded

- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_logic/bwsc_energy_telem/`
- ITTIA QNX and product pages listed in `VSN-0020`

## External Guidance

- RTI MCP: unavailable. DDS telemetry input/output contracts are blocked until
  RTI guidance is recorded.
- QNX online docs: use roadmap QNX docs for filesystem/runtime behavior.
- ITTIA DB: ITTIA QNX page states ITTIA DB supports QNX time-series data
  ingestion, queries, and real-time analytics. Product-specific C/C++ API docs
  must be obtained locally before implementation.

## Proposed Architecture

- Package name: generic, for example `evsn_energy_telemetry`.
- DDS inputs: typed MPPT, BMS, motor telemetry, odometer, and solar current
  snapshots.
- Database adapter: ITTIA DB C/C++ wrapper with explicit open, schema migrate,
  read latest, write sample, reset accumulator, compact/export, and close
  statuses.
- Persistence: local QNX path configured outside source, no cloud dependency.
- Runtime: bounded write queue outside real-time paths; explicit drop/backlog
  diagnostics.
- Schema: generic names such as `solar_energy_wh`, `battery_energy_wh`, and
  `vehicle_energy_totals`, not `car_bwsc_*`.

## Subagents and Skills

- `researcher`: map old energy calculations and InfluxDB measurements.
- `implementer`: port pure energy accumulator, ITTIA adapter, DDS adapter, and
  tests.
- `reviewer`: verify no InfluxDB/BWSC naming remains and DB failure behavior is
  bounded.
- Required skills: `qnx-cpp-rtos`, `rti-connext-dds`,
  `middleware-testing`, `cpp-unit-testing`, `cpp-integration-testing`,
  `test-coverage-evidence`, `safety-critical-cpp`.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on energy accumulator unit tests alone.

Minimum evidence:

- smoke test for telemetry startup with fake DDS inputs and fake DB adapter
- unit tests for accumulation, reset, timebase handling, bounds, stale inputs,
  invalid samples, and error propagation
- fake DB integration tests for open/schema/write/readback/reset/close,
  full/locked/error cases, and replay
- DDS integration tests with real RTI entities after the RTI gate clears, plus
  QNX ITTIA smoke only after SDK/API guidance is available
- regression tests proving InfluxDB and BWSC/SR/Sunswift naming are absent and
  DB writes stay outside real-time paths

## Steps

- [ ] Define generic energy IDL and topic names under `VSN-0021`.
- [ ] Obtain local ITTIA DB C/C++ API documentation and record license/build
      assumptions.
- [ ] Rewrite accumulator logic in pure C++.
- [ ] Implement fake DB adapter for host tests.
- [ ] Implement ITTIA DB adapter for QNX after docs are available.
- [ ] Add DDS adapter after RTI MCP guidance is recorded.
- [ ] Add reset/restore tests with durable latest-value behavior.

## Verification

- [ ] Smoke: starts with fake DB and fake DDS inputs, validates schema, exits.
- [ ] Unit: energy integration, reset, latest-value restore, invalid samples,
      overflow/bounds, and stale data.
- [ ] Integration: DDS telemetry input to fake DB persisted rows.
- [ ] QNX target: ITTIA DB open/write/read/reset smoke when SDK is available.

## Evidence

- Planned log: `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

Depends on `VSN-0021`, `VSN-0027`, and RTI MCP guidance. QNX ITTIA
implementation also depends on local ITTIA SDK/API documentation.
