# 2026-06-20 - RTI Perftest Regression Harness Plan

Work item: `VSN-0034`
Status: planned before implementation

## Actions

- Resolved the requested `rtinetworkperftest` source to RTI's public
  `rticommunity/rtiperftest` repository, branded as RTI Perftest / RTI's
  Network Performance Benchmarking Tool.
- Created `.agents/exec_plans/rtiperftest-regression-harness.md`.
- Added `VSN-0034` to `.agents/feature_list.json`.
- Added the pre-existing `VSN-0033` regression-suite plan to
  `.agents/feature_list.json` so `VSN-0034` can depend on it without an unknown
  work-item reference.

## RTI MCP Evidence

Initial broad query timed out after approximately 120 seconds.

Follow-up query:

> For RTI Connext Perftest in a CI harness, what key precautions should we
> record: isolated DDS domain, transport selection, not sharing app QoS/
> contracts, publisher/subscriber startup order, and interpreting latency/
> throughput as performance evidence rather than functional correctness?

Relevant result:

- Use an isolated DDS domain.
- Explicitly select and document transports and NICs.
- Keep benchmark QoS/settings separate from application contracts unless
  intentionally reproducing a production scenario.
- Start subscriber first, allow discovery/matching, then start publisher unless
  testing late-joiner behavior.
- Treat latency/throughput as performance evidence and trend data, not proof of
  functional correctness.
- Make scenarios representative and record variability, not just averages.

Design consequence:

- The plan requires dedicated domains, explicit transport/NIC selection,
  benchmark-only configuration, deterministic startup, evidence capture, and
  separation between functional regression pass/fail and performance trends.

## QNX Online Documentation Evidence

Consulted:

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

Design consequence:

- QNX Perftest runs are planned as opt-in target-tier work with explicit target,
  architecture, runtime library path, deployment, execution, log capture, and
  cleanup evidence.

## RTI Perftest Source Evidence

Consulted:

- `https://github.com/rticommunity/rtiperftest`
- `https://community.rti.com/static/documentation/perftest/current/index.html`
- `https://github.com/rticommunity/rtiperftest/releases`

Relevant facts:

- The GitHub repository describes RTI Perftest as RTI's Network Performance
  Benchmarking Tool.
- RTI documentation describes Perftest as measuring minimum latency, maximum
  throughput, and loaded latency.
- RTI documentation describes the publisher/subscriber ping-pong mechanism used
  to derive round-trip and one-way latency.
- RTI documentation says `build.sh` supports Linux, macOS, QNX, VxWorks, Lynx,
  and Android.
- The GitHub releases page reported `4.3` as latest on 2026-04-20, including
  Connext 7.7.0 compatibility, CPU thread affinity control, and QNX-related
  fixes.

## Verification

- `jq empty .agents/feature_list.json` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 34 work items and links in 119 Markdown
  files.
