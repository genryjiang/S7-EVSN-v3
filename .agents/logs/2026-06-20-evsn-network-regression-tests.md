# 2026-06-20 - EVSN Network Regression Tests

Work item: `VSN-0033`

## Actions

- Loaded harness and testing skills for strict verification planning:
  `harness-creator`, `test-coverage-evidence`, `cpp-integration-testing`, and
  `middleware-testing`.
- Read coverage, test automation, simulation/SIL/HIL, DDS middleware, and MQTT
  middleware testing references from the selected skills.
- Created or strengthened `.agents/exec_plans/evsn-network-regression-test-suite.md`
  as the separate regression and integration test-suite plan for the network
  migration.
- Added `Strict Verification Gate` sections to the network implementation
  plans, including the CAN HAL, generic CAN gateway, IDL/contracts, CAN broker,
  motor-control critical path, drivers, controllers, ITTIA telemetry, and cloud
  bridge plans.
- Updated `.agents/feature_list.json` so `VSN-0020` records the separate
  regression-suite gate and existing `VSN-0033` records stricter acceptance
  criteria, dependencies, blockers, and evidence.

## Design Consequence

- No network package should be called complete on smoke or unit tests alone.
- Each implementation plan must link package-level smoke, unit, integration,
  negative regression, and evidence records to `VSN-0033`.
- DDS integration tests remain subject to the RTI Connext MCP gate.
- QNX/SIL and QNX target tiers remain subject to QNX documentation, toolchain,
  and target-contract evidence.
- CAN0 final motor-command routing must be covered by negative regression tests
  proving it cannot pass through DDS, the generic CAN gateway, telemetry, cloud,
  database, or filesystem paths.

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `./init.sh check` passed, validating 34 work items and links in 119 Markdown
  files.
- `git diff --check` passed.
