---
name: cpp-integration-testing
description: General C++ integration testing across modules, processes, middleware adapters, QNX services, and harness components. Use for host integration, SIL/emulation, QNX-target, hardware-adjacent, startup/shutdown, reconnect, retry, malformed input, stale data, timeout, and degraded-mode tests.
---

# C++ Integration Testing

Use this skill for integration tests that cross module, process, middleware, target, or harness boundaries.

## Core Guidance

- Distinguish host integration, SIL/emulation, QNX-target, and hardware-adjacent tests.
- Use deterministic timeouts and readiness handshakes instead of arbitrary sleeps.
- Test startup order, shutdown order, reconnect behavior, retry behavior, malformed input, stale data, timeout handling, and degraded-mode behavior.
- Record which platform, target, middleware configuration, transport, and build configuration were tested.
- Capture logs, traces, inputs, outputs, and exit status sufficient to debug failures.
- Do not claim ISO 26262 compliance from integration tests alone.

## References

Read the relevant references before designing integration, simulation, or target test coverage:

- `references/test-automation-rules.md`
- `references/simulation-testing-rules.md`
- `references/coverage-criteria-rules.md`
