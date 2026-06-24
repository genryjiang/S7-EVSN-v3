# VSN-0034 - RTI Perftest Regression Harness

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Create an EVSN test harness that introduces RTI's public Perftest repository as
a controlled DDS performance-characterization tool that can run before, after,
or around the network regression suite.

The upstream source requested as `rtinetworkperftest` is recorded as
`rticommunity/rtiperftest`, branded by RTI as RTI Perftest / RTI's Network
Performance Benchmarking Tool:

- `https://github.com/rticommunity/rtiperftest`
- `https://community.rti.com/static/documentation/perftest/current/index.html`

The harness must support:

- pinned upstream source by tag, commit, or approved release archive
- Linux builder execution using the existing RTI Connext environment
- optional QNX target execution after a target contract exists
- explicit `before`, `after`, `both`, and `off` modes relative to regression
  tests
- raw evidence capture plus machine-readable summaries for trend comparison
- deterministic startup, bounded runtime, cleanup, and failure reporting

## Non-Goals

- Do not use RTI Perftest as proof of EVSN functional correctness.
- Do not replace package smoke, unit, DDS integration, adapter integration, or
  network regression tests from `VSN-0033`.
- Do not change production DDS IDL, topic names, QoS profiles, participants,
  WaitSet/listener paths, or application transport settings for this harness.
- Do not run QNX target tests by default or without an approved target contract.
- Do not claim MISRA C++ compliance, ISO 26262 compliance, or vehicle readiness
  from performance-test evidence.
- Do not import or modify upstream Perftest source without a recorded license,
  provenance, and update policy.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/exec_plans/README.md`
- `.agents/exec_plans/evsn-network-regression-test-suite.md`
- `init.sh`
- `scripts/verify`
- `scripts/linux-test`
- `.agents/skills/rti-connext-dds/SKILL.md`
- `.agents/skills/rti-connext-dds/references/dds-middleware.yaml`
- `.agents/skills/cpp-integration-testing/SKILL.md`
- `.agents/skills/cpp-integration-testing/references/test-automation-rules.md`
- `.agents/skills/cpp-integration-testing/references/simulation-testing-rules.md`
- `.agents/skills/cpp-integration-testing/references/coverage-criteria-rules.md`

## External Guidance

RTI MCP:

- Initial broad query timed out after approximately 120 seconds:
  "We are planning a repository test harness for an embedded QNX/RTI Connext C++
  project. The harness will integrate RTI's public Perftest repository
  (rticommunity/rtiperftest, RTI Network Performance Benchmarking Tool) as an
  optional pre- or post-regression performance characterization gate. It should
  run publisher/subscriber pairs around the existing regression tests, capture
  latency/throughput evidence, avoid changing production DDS topic/type/QoS
  contracts, and support Linux builder runs plus optional QNX target runs. What
  RTI Connext-specific guidance should the execution plan record for building/
  running RTI Perftest, choosing domains/transports/QoS, isolating it from
  application participants, interpreting results, and avoiding misuse as a
  functional regression pass/fail gate?"
- Follow-up narrow query succeeded:
  "For RTI Connext Perftest in a CI harness, what key precautions should we
  record: isolated DDS domain, transport selection, not sharing app QoS/
  contracts, publisher/subscriber startup order, and interpreting latency/
  throughput as performance evidence rather than functional correctness?"
- Relevant result:
  - run Perftest in an isolated DDS domain
  - explicitly select and document transports and NICs
  - keep benchmark QoS/settings separate from application DDS contracts unless
    intentionally reproducing a scenario
  - start subscriber first, wait for discovery/matching, then start publisher
    unless late-joiner behavior is under test
  - treat latency/throughput as performance evidence and trend data, not as
    functional correctness
  - make scenarios representative and record variability, not only averages
- Design consequence:
  - the harness will allocate a dedicated domain range, explicit transport/NIC
    configuration, bounded startup handshake, benchmark-only configuration, and
    evidence schema
  - `scripts/verify` integration must keep functional regression pass/fail
    separate from performance trend status

QNX online docs:

- QNX Product Documentation:
  `https://www.qnx.com/developers/docs/index.html`
- QNX SDP 8.0 documentation:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
- QNX SDP 8.0 Quickstart, compiling and linking:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.quickstart/topic/compiling.html`
- QNX SDP 8.0 Quickstart, running and debugging:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.quickstart/topic/starting.html`
- QNX SDP 8.0 Utilities Reference, selecting target system:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/targets.html`
- QNX SDP 8.0 Utilities Reference, commonly used environment variables:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/envars.html`
- Design consequence:
  - QNX Perftest runs must remain an opt-in target tier with explicit
    architecture, target environment, runtime library path, deployment,
    execution, log collection, and cleanup evidence
  - host/Linux performance runs cannot be treated as QNX target performance
    evidence

RTI Perftest public documentation and source:

- GitHub repository: `https://github.com/rticommunity/rtiperftest`
- RTI Perftest documentation:
  `https://community.rti.com/static/documentation/perftest/current/index.html`
- RTI Perftest releases:
  `https://github.com/rticommunity/rtiperftest/releases`
- Evidence notes:
  - the repository README describes RTI Perftest as RTI's network performance
    benchmarking tool
  - RTI documentation describes Perftest as measuring minimum latency, maximum
    throughput, and loaded latency
  - RTI documentation says Perftest works by having the publisher write data
    rapidly and periodically request an echo from the subscriber to derive RTT
    and one-way latency
  - RTI documentation lists support for Linux, macOS, QNX, VxWorks, Lynx, and
    Android through `build.sh`
  - the GitHub releases page reports latest release `4.3` on 2026-04-20, with
    Connext 7.7.0 compatibility, CPU thread affinity control, and QNX-related
    fixes

## Design Policy

- Source introduction:
  - Prefer an explicit Git submodule under `third_party/rtiperftest` or a
    locked release archive fetch under a deterministic cache.
  - Pin the exact upstream tag/commit/archive checksum.
  - Record EPL-1.0 license provenance and any local distribution constraints.
  - Do not edit upstream source in place; keep any wrapper patches separate and
    documented.
- Build:
  - Build Perftest inside the existing RTI builder environment using
    `NDDSHOME`, `CONNEXTDDS_ARCH`, and the upstream `build.sh`.
  - The first implementation should target Linux builder execution only.
  - QNX builds require the repository QNX builder, target RTI libraries, and an
    approved target architecture.
- Execution:
  - Add a wrapper such as `scripts/rtiperftest-harness`.
  - Support `--phase before|after|both|off`, with matching environment variable
    `EVSN_RTI_PERFTEST_PHASE`.
  - Use `off` or non-gating trend-only behavior for ordinary local
    `./init.sh verify` unless a maintainer opts in.
  - Let the full network regression job choose `before`, `after`, or `both`.
  - Run the subscriber first, wait for readiness/discovery evidence, then run
    the publisher.
  - Use a dedicated DDS domain range, never application domains.
  - Require explicit `--transport`, `--nic`, `--data-len`, `--duration`, and
    `--domain` selection or documented defaults.
  - Bound execution time and kill both publisher/subscriber on timeout.
- Evidence:
  - Store results under `.agents/evidence/VSN-0034/<timestamp>/`.
  - Capture command lines, RTI Perftest tag/commit, Connext version, platform,
    target, architecture, transport, NIC, domain ID, payload size, duration,
    raw publisher/subscriber output, parsed latency/throughput metrics, exit
    status, and cleanup status.
  - Compare to an approved baseline only after enough repeatable data exists.
  - Treat threshold failure as performance-regression evidence, not as a
    substitute for functional regression failure.

## Integration Shape

Preferred command surface:

- `scripts/rtiperftest-harness --dry-run --phase before`
- `scripts/rtiperftest-harness --dry-run --phase after`
- `EVSN_RTI_PERFTEST_PHASE=after ./scripts/verify`
- `EVSN_RTI_PERFTEST_PHASE=both ./scripts/verify`
- optional target tier:
  `EVSN_RTI_PERFTEST_TARGET=qnx ./scripts/rtiperftest-harness --phase after`

Preferred `scripts/verify` behavior:

1. `git diff --check`
2. shell syntax checks
3. optional RTI Perftest `before` phase
4. `scripts/linux-test --clean`
5. optional RTI Perftest `after` phase
6. optional QNX cross-build when `VERIFY_QNX=1`
7. optional QNX Perftest target run only when a target contract is configured

The wrapper must leave existing `scripts/linux-test` and CTest behavior
unchanged except for the explicit optional pre/post hook.

## Steps

- [ ] Decide and document source strategy: Git submodule, pinned archive fetch,
      or preinstalled binary artifact.
- [ ] Record license/provenance for RTI Perftest and the selected source pin.
- [ ] Add ignored build/evidence/cache paths for Perftest outputs.
- [ ] Add `scripts/rtiperftest-harness` with `--dry-run`, `--phase`, `--domain`,
      `--transport`, `--nic`, `--data-len`, `--duration`, `--evidence-dir`,
      and target selection.
- [ ] Add a Linux builder build path that invokes upstream `build.sh` using the
      existing RTI environment.
- [ ] Add a Linux builder execution path that starts subscriber first,
      confirms readiness, starts publisher, captures output, enforces timeout,
      and cleans up both processes.
- [ ] Add parser logic for Perftest output summaries without depending on a
      fragile single-line format.
- [ ] Add evidence summary generation.
- [ ] Add `scripts/verify` optional before/after integration without changing
      default functional regression semantics.
- [ ] Add deterministic dry-run and parser unit tests.
- [ ] Add a smoke run in the Linux builder using short bounded duration and an
      isolated domain.
- [ ] Add optional QNX target plan extension only after target contract,
      deployment path, environment, RTI QNX libraries, and cleanup behavior are
      approved and documented.

## Verification

- [x] Command: `./init.sh check`
      Result: Passed on 2026-06-20; validated 34 work items and links in 119
      Markdown files.
- [x] Command: `git diff --check`
      Result: Passed on 2026-06-20.
- [ ] Command: `./scripts/rtiperftest-harness --dry-run --phase before`
      Result:
- [ ] Command: `./scripts/rtiperftest-harness --dry-run --phase after`
      Result:
- [ ] Command: parser unit tests for representative Perftest publisher and
      subscriber output
      Result:
- [ ] Command: `EVSN_RTI_PERFTEST_PHASE=after ./scripts/verify`
      Result:
- [ ] Command: bounded Linux builder Perftest smoke run
      Result:
- [ ] Command: optional QNX target Perftest smoke run
      Result:

## Evidence

- Log: `.agents/logs/2026-06-20-rtiperftest-regression-harness.md`
- Work item: `VSN-0034`
- Regression-suite dependency: `VSN-0033`

## Handoff

Implementation should start with Linux-only dry-run/build/run support. Keep the
default verification gate non-gating for performance until maintainers approve
baseline thresholds. QNX target execution remains a separate opt-in extension
with target-specific evidence.
